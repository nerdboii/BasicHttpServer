#pragma once

#include <sys/epoll.h>
#include <vector>

/**
 * @brief Stores data of an event in epoll_event.ptr
 * 
 */
struct EventData {
    EventData() : fd(0), length(0), current(0), buf() {}
    int fd;             // file descriptor
    size_t length;      // length of the buffer
    size_t current;     // current position, first half sent, second half remaining
    char buf[1000];     // buffer of the event
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
     * @brief Add a file descriptor to the epoll instance for monitoring
     * 
     * @param fd The file descriptor to be added
     */
    void add(int control_operator, int fd, uint32_t events, void* data);

    /**
     * @brief Remove a file descriptor fromt the epoll instance
     * 
     * @param fd The file descriptor to be removed
     */
    void remove(int fd);

    /**
     * @brief Wait for I/O events on registered file descriptor
     * 
     * @param timeout The maximum time to wait in milliseconds
     *                -1 means wait indefinitely
     * @return std::vector<epoll_event> List of triggered events
     *                                  Usually its new connections or new requests
     */
    std::vector<epoll_event> wait(int timeout = -1);

    /**
     * @brief control the coming event to be added or deleted
     * 
     * @param ctrl_operator control operator, decides whether to add or to delete this event
     * @param fd event file descriptor
     * @param events represents type of the  event
     * @param data data of the event, including fd, length, buffer and current position in buffer
     */
    void controlEvent(int ctrl_operator, int fd, uint32_t events, void* data);

private:
    int epoll_fd;   // File descriptor of the epoll instance
};