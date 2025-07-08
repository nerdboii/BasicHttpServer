#include "HttpResponse.h"

std::string HttpResponse::buildResponse(const std::string& content) {
    std::string response = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: " + std::to_string(content.size()) + "\r\n"
        "Connection: close\r\n\r\n"
        + content;
    return response;
}