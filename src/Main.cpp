#include "Server.h"

#include <iostream>
#include <sys/resource.h>
#include <string.h>

void ensure_enough_resource(int resource, std::uint32_t soft_limit, std::uint32_t hard_limit) {
    rlimit new_limit, old_limit;

    new_limit.rlim_cur = soft_limit;
    new_limit.rlim_max = hard_limit;
    getrlimit(resource, &old_limit);

    std::cout << "Old limit: " << old_limit.rlim_cur << " (soft limit), "
    << old_limit.rlim_cur << " (hard limit)." << std::endl;
    std::cout << "New limit: " << new_limit.rlim_cur << " (soft limit), "
    << new_limit.rlim_cur << " (hard limit)." << std::endl;

    if (setrlimit(resource, &new_limit)) {
        std::cerr << "Warning: Could not update resource limit ";
        std::cerr << "(" << strerror(errno) << ")." << std::endl;
        std::cerr << "Consider setting the limit manually with ulimit" << std::endl;
        exit(-1);
    }
}

int main() {
    //std::cout << "Setting new limits for number of threads.." << std::endl;
    //ensure_enough_resource(RLIMIT_NPROC, 60000, 60000);

    Server server(8080);
    server.run();

    std::cout << "Enter [quit] to stop the server" << std::endl;
    std::string inputCmd;
    while (std::cin >> inputCmd, inputCmd != "quit") {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "[quit] command entered. Stop the server" << std::endl;
    server.stop();
    std::cout << "Server stopped!" << std::endl;
    return 0;
}