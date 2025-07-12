#pragma once

#include <sys/epoll.h>
#include <vector>
#include <string>
#include <atomic>

constexpr size_t maxBufferSize = 4096;
constexpr size_t maxEvents = 10000;

struct EventData {
  EventData() : fd(0), length(0), cursor(0), buffer() {}
  int fd;
  size_t length;
  size_t cursor;
  char buffer[maxBufferSize];
};

  
/**
 * @brief Wrapper of epoll APIs from Linux, helps managing client connections
 * 
 * This class creates an epoll instance and manage it with the epoll_fd
 * Can add, remove, get requests from clients
 */
class Epoll {
public:
    /**
     * @brief Construct a new Epoll object
     * 
     */
    Epoll();

    /**
     * @brief Destroy the Epoll object
     * 
     */
    ~Epoll();

    /**
     * @brief modify the events
     * 
     * @param client_fd event's file descriptor
     * @param events to be modified
     */
    void modify(int client_fd, uint32_t events) { modify(client_fd, events); }

    /**
     * @brief Wait for I/O events on registered file descriptor
     * 
     * @param timeout The maximum time to wait in milliseconds
     *                -1 means wait indefinitely
     * @return epoll_event[] List of triggered events
     *                                  Usually its new connections or new requests
     */
    int wait(epoll_event* workerEvent, int timeout = 0);

    /**
     * @brief control the coming event to be added or deleted
     * 
     * @param ctrl_operator control operator, decides whether to add or to delete this event
     * @param fd event file descriptor
     * @param events represents type of the  event
     * @param data data of the event, including fd, length, buffer and current position in buffer
     */
    void controlEvent(int ctrl_operator, int fd, uint32_t events, void* data, std::atomic<int>& client_count);

private:
    int epoll_fd;   // File descriptor of the epoll instance
};