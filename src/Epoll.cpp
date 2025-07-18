#include "Epoll.h"

#include <unistd.h>
#include <stdexcept>

Epoll::Epoll() {
    epoll_fd = epoll_create1(0);
    if (epoll_fd < 0) throw std::runtime_error("Failed to create epoll");
}

Epoll::~Epoll() {
    close(epoll_fd);
}

int Epoll::wait(epoll_event* workerEvent, int timeout) {
    return epoll_wait(epoll_fd, workerEvent, maxEvents, timeout);
}

void Epoll::controlEvent(int ctrl_operation, int fd, std::uint32_t events, void *data, std::atomic<int>& client_count) {

    if (ctrl_operation == EPOLL_CTL_DEL) {
        if (epoll_ctl(epoll_fd, ctrl_operation, fd, nullptr) < 0) {
          throw std::runtime_error("Failed to remove file descriptor");
        }
        client_count--;
      } else {
        epoll_event ev;
        ev.events = events;
        ev.data.ptr = data;
        if (epoll_ctl(epoll_fd, ctrl_operation, fd, &ev) < 0) {
          throw std::runtime_error("Failed to add or modify file descriptor");
        }
        if (ctrl_operation == EPOLL_CTL_ADD) client_count++;
      }
}
