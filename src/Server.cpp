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
    setupServerSocket();
    epoll.add(server_fd);
}

Server::~Server() {
    close(server_fd);
}

void Server::setupServerSocket() {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
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
    while (true) {
        int client_fd = accept(server_fd, nullptr, nullptr);
        if (client_fd == -1) break;
        fcntl(client_fd, F_SETFL, O_NONBLOCK);
        epoll.add(client_fd);
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

    std::string response = HttpResponse::buildResponse(content);
    send(client_fd, response.c_str(), response.size(), 0);
    close(client_fd);
}
