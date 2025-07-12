#include "IRequestHandler.h"
#include "Epoll.h"
#include "URI.h"
#include "WorkerThread.h"

#include <unordered_set>
#include <thread>
#include <atomic>

/**
 * @brief HTTP server class using epoll for handling multiple client connections.
 * 
 * This class sets up a TCP server socket, accepts incoming client connections,
 * and handles requests asynchronously via epoll.
 */
class Server : public IRequestHandler {
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

    void stop();

    /**
     * @brief Set up the socket server
     * 
     * Create the socket, set it to non-blocking mode, enable address reuse,
     * bind it to the specified port, and start listening
     */
    void setupServerSocket();

    /**
     * @brief Handle a HttpRequest by server
     * 
     * @param request 
     * @return HttpResponse response to the given request
     */
    HttpResponse handleRequest(const HttpRequest& request) override;

    // Get least loaded thread in worker threads list
    WorkerThread* pickLeastLoadedThread();

    // Listens to new connection
    void clientListener();

private:
    static const int threadPoolSize = 5;
    static const int backlogSize = 8192;

    int server_fd;  // File descriptor for the server socket
    int port;       // Port number the server will bind to
    std::unordered_set<URI> supportedURI;
                    // Set of URIs supported by this server
    std::unordered_set<HttpMethod> supportedMethod;
                    // Set of Methods supported by this server
    std::thread listenerThread;
    WorkerThread *workerThreads[threadPoolSize];
    bool running;
};