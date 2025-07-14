#include "WorkerThread.h"
#include "HttpRequest.h"

#include <string.h>
#include <stdexcept>

void WorkerThread::threadLoop() {
    EventData* data;
    bool active = true;

    while (running) {
        if (!active) {
            auto& rng = RandomUtils::rng();
            auto& sleep_times = RandomUtils::sleepTimes();
            int sleep_time = sleep_times(rng);
            std::this_thread::sleep_for(std::chrono::microseconds(sleep_time));
        }

        int cnt = epollInstance.wait(events);

        if (cnt <= 0) {
            active = false;
            continue;
        }
      
        active = true;

        // handle events
        for (int i = 0; i < cnt; i++) {
            auto event = events[i];
            data = reinterpret_cast<EventData *>(event.data.ptr);
            if (event.events & (EPOLLHUP | EPOLLERR)) {
                controlEvent(EPOLL_CTL_DEL, data->fd, 0, nullptr);
                close(data->fd);
                delete data;
            } else if (event.events & (EPOLLIN | EPOLLOUT)) {
                handleEpollEvent(data, event.events);
            } else {
                controlEvent(EPOLL_CTL_DEL, data->fd, 0, nullptr);
                close(data->fd);
                delete data;
            }
        }
    }
}

void WorkerThread::handleEpollEvent(EventData* data, uint32_t events) {
    int client_fd = data->fd;
    EventData *request, *response;
    ssize_t bytes;

    if (events == EPOLLIN) { // handling reading event
        request = data;
        bytes = recv(client_fd, request->buffer, maxBufferSize, 0);

        if (bytes > 0) { // since i only support GET and HEAD method
                         // i'll assume the server to be able fully received the message everytime i read
            response = new EventData();
            response->fd = client_fd;
            handleHttpData(*request, response);
            controlEvent(EPOLL_CTL_MOD, client_fd, EPOLLOUT, response);
            delete request;
        } else if (bytes == 0) { // client closed connection
            controlEvent(EPOLL_CTL_DEL, client_fd, 0, nullptr);
            close(client_fd);
            delete request;
        } else {
            if (errno == EAGAIN || errno == EWOULDBLOCK) { // retry
                request->fd = client_fd;
                controlEvent(EPOLL_CTL_MOD, client_fd, EPOLLIN, request);
            } else { // unexpected error
                controlEvent(EPOLL_CTL_DEL, client_fd, 0, nullptr);
                close(client_fd);
                delete request;
            }
        }
    } else { // handling writing event
        response = data;
        bytes = send(client_fd, response->buffer + response->cursor, response->length, 0);
        if (bytes >= 0) {
            if (bytes < response->length) { // message remaining
                response->cursor += bytes;  // move the cursor to continue reading next bytes
                response->length -= bytes;  // update remaining bytes
                controlEvent(EPOLL_CTL_MOD, client_fd, EPOLLOUT, response);
            } else {                           // message is completely written
                request = new EventData();  // make a new request when modifying this event to EPOLLIN
                request->fd = client_fd;
                controlEvent(EPOLL_CTL_MOD, client_fd, EPOLLIN, request);
                                            // modify the current event to EPOLLIN to read the upcoming request
                delete response;
            }
        } else {
            if (errno == EAGAIN || errno == EWOULDBLOCK) { // retry
                controlEvent(EPOLL_CTL_ADD, client_fd, EPOLLOUT, response);
            } else { // unexpected error
                controlEvent(EPOLL_CTL_DEL, client_fd, 0, nullptr);
                close(client_fd);
                delete response;
            }
        }
    }
}

void WorkerThread::handleHttpData(const EventData &raw_request, EventData *raw_response) {
    std::string request_string(raw_request.buffer), response_string;
    HttpRequest http_request;
    HttpResponse http_response;

    try {
        http_request = string_to_request(request_string);
        http_response = handler->handleRequest(http_request);
    } catch (const std::invalid_argument &e) {
        http_response = HttpResponse(HttpStatusCode::BadRequest);
        http_response.setContent(e.what());
    } catch (const std::logic_error &e) {
        http_response = HttpResponse(HttpStatusCode::HttpVersionNotSupported);
        http_response.setContent(e.what());
    } catch (const std::exception &e) {
        http_response = HttpResponse(HttpStatusCode::InternalServerError);
        http_response.setContent(e.what());
    }

    // Set response to write to client
    response_string =
    to_string(http_response, http_request.getMethod() != HttpMethod::HEAD);
    memcpy(raw_response->buffer, response_string.c_str(), maxBufferSize);
    raw_response->length = response_string.length();
}