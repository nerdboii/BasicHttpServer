#include "Epoll.h"
#include <unistd.h>
#include <stdexcept>

Epoll::Epoll() {
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) throw std::runtime_error("Failed to create epoll");
}

Epoll::~Epoll() {
    close(epoll_fd);
}

void Epoll::add(int fd) {
    epoll_event event;
    // Assign the event by this file descriptor
    event.data.fd = fd;
    // Make the event to notify once the fd is readable and everytime there's data to read from it
    event.events = EPOLLIN | EPOLLET;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event);
}

void Epoll::remove(int fd) {
    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, nullptr);
}

std::vector<epoll_event> Epoll::wait(int timeout) {
    std::vector<epoll_event> events(1024);
    int n = epoll_wait(epoll_fd, events.data(), events.size(), timeout);
    events.resize(n);
    return events;
}