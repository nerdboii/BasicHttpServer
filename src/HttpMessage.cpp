#include "HttpMessage.h"
#include "StringProcessor.h"

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

