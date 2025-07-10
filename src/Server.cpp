#include "Server.h"
#include "HttpParser.h"
#include "HttpResponse.h"

#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

Server::Server(int port) : port(port) {
    supportedMethod.insert(HttpMethod::GET);
    supportedMethod.insert(HttpMethod::HEAD);
    supportedURI.insert(URI("/"));
    supportedURI.insert(URI("/about"));
    setupServerSocket();
    epoll.add(EPOLL_CTL_ADD, server_fd, EPOLLIN, nullptr);
}

Server::~Server() {
    close(server_fd);
}

void Server::setupServerSocket() {
    server_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    fcntl(server_fd, F_SETFL, O_NONBLOCK);

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr {};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    bind(server_fd, (sockaddr*)&addr, sizeof(addr));
    listen(server_fd, SOMAXCONN);
}

void Server::run() {
    std::cout << "Server starts, listening to new connection" << std::endl;
    while (true) {
        // Get event list from kernel since the last wait
        auto events = epoll.wait();
        for (auto& event : events) {
            // data.fd = serfer_fd means that there's a new connection need to be accepted
            // Otherwise its a request from a connected client
            if (event.data.fd == server_fd) {
                acceptConnection();
            } else {
                handleClient(event.data.fd);
            }
        }
    }
}

void Server::acceptConnection() {
    EventData *data;
    sockaddr client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd;

    while (true) {
        int client_fd = accept4(server_fd, &client_addr, &client_len, SOCK_NONBLOCK);
        if (client_fd < 0) break;
        data = new EventData();
        data->fd = client_fd;
        epoll.add(EPOLL_CTL_ADD, client_fd, EPOLLIN, data);
    }
}

void Server::handleClient(int client_fd) {
    char buffer[4096];
    int bytes = read(client_fd, buffer, sizeof(buffer));
    if (bytes <= 0) {
        close(client_fd);
        return;
    }

    std::string request(buffer, bytes);
    std::string path = HttpParser::getRequestPath(request);

    std::string content = "<html><h1>Hello from C++ HTTP Server!</h1></html>";
    if (path == "/about") content = "<html><h1>About Page</h1></html>";

    std::string response;//= HttpResponse::buildResponse(content);
    send(client_fd, response.c_str(), response.size(), 0);
    close(client_fd);
}

HttpResponse Server::handleRequest(const HttpRequest& request) {
    if (supportedURI.find(request.getUri()) == supportedURI.end()) {
        return HttpResponse(HttpStatusCode::NotFound);
    }
    if (supportedMethod.find(request.getMethod()) == supportedMethod.end()) {
        return HttpResponse(HttpStatusCode::MethodNotAllowed);
    }
    HttpResponse response(HttpStatusCode::Ok);
    switch (request.getMethod()) {
        // Implementing this
        case HttpMethod::GET:
            response.setHeader("Content-Type", "text/plain");
            response.setContent("Hello, world\n");
            return response;
        case HttpMethod::HEAD:
            return;
        default:
            return;
    }
}