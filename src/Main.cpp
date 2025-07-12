#include "Server.h"

#include <iostream>

int main() {
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