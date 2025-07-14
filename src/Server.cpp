#include "Server.h"

#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include <algorithm>
#include <limits.h>

Server::Server(int port) : port(port) {
    supportedMethod.insert(HttpMethod::GET);
    supportedMethod.insert(HttpMethod::HEAD);
    supportedURI.insert(URI("/"));
    supportedURI.insert(URI("/about"));
    running = false;
}

Server::~Server() {
    for (int i = 0; i < threadPoolSize; i++) delete workerThreads[i];
}

void Server::setupServerSocket() {
    server_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    fcntl(server_fd, F_SETFL, O_NONBLOCK);

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
        throw std::runtime_error("Failed to set socket options");
    }

    sockaddr_in addr {};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(server_fd, (sockaddr*)&addr, sizeof(addr)) < 0) {
        throw std::runtime_error("Failed to bind to socket");
    }
}

void Server::run() {
    std::cout << "Server starts, listening to new connection on port: " << port << std::endl;

    setupServerSocket();

    if (listen(server_fd, backlogSize) < 0) {
        std::ostringstream msg;
        msg << "Failed to listen on port " << port;
        throw std::runtime_error(msg.str());
    }

    running = true;
    listenerThread = std::thread(&Server::clientListener, this);

    for (int i = 0; i < threadPoolSize; i++) {
        workerThreads[i] = new WorkerThread(this);
        workerThreads[i]->start();
    }
}

void Server::stop() {
    running = false;
    listenerThread.join();
    
    for (int i = 0; i < threadPoolSize; i++) {
        workerThreads[i]->join();
    }

    close(server_fd);
}

HttpResponse Server::handleRequest(const HttpRequest& request) {
    if (supportedURI.find(request.getUri()) == supportedURI.end()) {
        return HttpResponse(HttpStatusCode::NotFound);
    }
    if (supportedMethod.find(request.getMethod()) == supportedMethod.end()) {
        return HttpResponse(HttpStatusCode::MethodNotAllowed);
    }

    HttpResponse response(HttpStatusCode::Ok);
    if (request.getUri().getPath() == "/") {
        response.setHeader("Content-Type", "text/plain");
        response.setContent("Hello from server\n");
    } else if (request.getUri().getPath() == "/about") {
        std::string content = "";
        content += "<!doctype html>\n";
        content += "<html>\n<body>\n\n";
        content += "<h1>About Page</h1>\n";
        content += "</body>\n</html>\n";
        
        response.setHeader("Content-Type", "text/html");
        response.setContent(content);
    }

    return response;
}

WorkerThread* Server::pickLeastLoadedThread() {
    int minClient = INT_MIN, workerIndex = 0;
    for (int i = 0; i < threadPoolSize; i++) {
        if (workerThreads[i]->countClient() < minClient) {
            workerIndex = i;
            minClient = workerThreads[i]->countClient();
        }
    }
    return workerThreads[workerIndex];
}

void Server::clientListener() {
    EventData *client_data;
    sockaddr_in client_address;
    socklen_t client_len = sizeof(client_address);
    int client_fd;
    bool active = true;
  
    // accept new connections and distribute tasks to worker threads
    while (running) {
        if (!active) {
            // avoid contention
            auto& rng = RandomUtils::rng();
            auto& sleep_times = RandomUtils::sleepTimes();
            int sleep_time = sleep_times(rng);
            std::this_thread::sleep_for(std::chrono::microseconds(sleep_time));
        }
        client_fd = accept4(server_fd, (sockaddr *)&client_address, &client_len, SOCK_NONBLOCK);
        if (client_fd < 0) {
            active = false;
            continue;
        }
  
        active = true;
        client_data = new EventData();
        client_data->fd = client_fd;
        WorkerThread* worker = pickLeastLoadedThread();
        worker->controlEvent(EPOLL_CTL_ADD, client_fd, EPOLLIN, client_data);
    }
}