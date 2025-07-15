#include "HttpMessage.h"
#include "StringProcessor.h"

#include <stdexcept>
#include <sstream>
#include <fstream>

std::string to_string(HttpVersion version) {
    switch (version) {
        case HttpVersion::HTTP_1_0:
            return "HTTP/1.0";
        case HttpVersion::HTTP_1_1:
            return "HTTP/1.1";
        default:
            return "";
    }
}

HttpVersion string_to_version(const std::string& versionString) {
    std::string temp = versionString;
    toUpper(temp);
    if (temp == "HTTP/1.0")
        return HttpVersion::HTTP_1_0;
    else if (temp == "HTTP/1.1")
        return HttpVersion::HTTP_1_1;
    else 
        throw std::logic_error("Unexpected HTTP version");
}

void HttpMessage::getContentFromFile(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        throw std::logic_error("Internal server error!"); // or throw if preferred
    }

    std::ostringstream ss;
    ss << file.rdbuf();
    content = ss.str();

    setContentLen();
}