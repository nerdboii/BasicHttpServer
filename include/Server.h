#pragma once

#include "Epoll.h"
#include "HttpRequest.h"
#include "URI.h"

#include <unordered_set>

/**
 * @brief HTTP server class using epoll for handling multiple client connections.
 * 
 * This class sets up a TCP server socket, accepts incoming client connections,
 * and handles requests asynchronously via epoll.
 */
class Server {
public:
    /**
     * @brief Construct a new Server object
     * 
     * @param port Port number to bind the server socket.
     */
    Server(int port);

    /**
     * @brief Destroy the Server object
     * 
     * Close server socket and release allocated resources.
     */
    ~Server();

    // Adding supported URI and Method
    void addSupportedURI(const URI& uri) { supportedURI.insert(uri); }
    void addSupportedMethod(const HttpMethod& method) { supportedMethod.insert(method); }

    /**
     * @brief Run the server's main loop
     * 
     * This method starts the epoll loop, waiting and processing events.
     */
    void run();

    /**
     * @brief Set up the socket server
     * 
     * Create the socket, set it to non-blocking mode, enable address reuse,
     * bind it to the specified port, and start listening
     */
    void setupServerSocket();

    /**
     * @brief Accept a new connection from a client
     * 
     * Accepts a new connection from a client then adds its file descriptor
     * to the epoll object
     */
    void acceptConnection();

    /**
     * @brief Handles a request from a connected client
     * 
     * Reads the client's request, processes it then sends a response
     * 
     * @param client_fd File descriptor of the requesting client socket
     */
    void handleClient(int client_fd);

    void handleEpollEvent();

    /**
     * @brief Handle a HttpRequest by server
     * 
     * @param request 
     * @return HttpResponse 
     */
    HttpResponse handleRequest(const HttpRequest& request);


private:
    int server_fd;  // File descriptor for the server socket
    int port;       // Port number the server will bind to
    Epoll epoll;    // Epoll object to manage connections to clients
    std::unordered_set<URI> supportedURI;
                    // Set of URIs supported by this server
    std::unordered_set<HttpMethod> supportedMethod;
                    // Set of Methods supported by this server
};