#include "HttpParser.h"

std::string HttpParser::getRequestPath(const std::string& request) {
    size_t method_end = request.find(' ');
    if (method_end == std::string::npos) return "/";

    size_t path_end = request.find(' ', method_end + 1);
    if (path_end == std::string::npos) return "/";

    return request.substr(method_end + 1, path_end - method_end - 1);
}