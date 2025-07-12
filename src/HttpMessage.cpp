#include "HttpMessage.h"
#include "StringProcessor.h"

#include <stdexcept>

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