#pragma once

#include <sys/epoll.h>
#include <vector>

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
    void add(int fd);

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

private:
    int epoll_fd;   // File descriptor of the epoll instance
};