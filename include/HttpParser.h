#pragma once

#include <string>

class HttpParser {
public:
    static std::string getRequestPath(const std::string& request);
};
