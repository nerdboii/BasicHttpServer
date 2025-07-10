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

void Epoll::add(int control_operator, int fd, uint32_t events, void* data) {
    epoll_event event;
    event.events = events;
    event.data.ptr = data;
    if (epoll_ctl(epoll_fd, control_operator, fd, &event) < 0) {
        throw std::runtime_error("Failed to add file descriptor");
    }
}

void Epoll::remove(int fd) {
    if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, nullptr) < 0) {
        throw std::runtime_error("Failed to remove file descriptor");
    }
}

std::vector<epoll_event> Epoll::wait(int timeout) {
    std::vector<epoll_event> events(1024);
    int n = epoll_wait(epoll_fd, events.data(), events.size(), timeout);
    events.resize(n);
    return events;
}

void Epoll::controlEvent(int control_operator, int fd, std::uint32_t events, void* data) {
    if (control_operator == EPOLL_CTL_DEL) {
        remove(fd);
    }
    else {
        add(control_operator, fd, events, data);
    }
}