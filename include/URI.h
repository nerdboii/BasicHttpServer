#pragma once

#include "StringProcessor.h"

#include <string>
#include <stdexcept>

enum class UriForm {
    ORIGIN,
    ABSOLUTE,
    ASTERISK,
    AUTHORITY
};

class URI {
public:
    URI() = default;
    URI(const std::string& path) : path(path) {
        toLower(this->path);
    }
    ~URI() = default;

    void setPath(const std::string& path) {
        this->path = std::move(path);
        toLower(this->path);
    }

    std::string getScheme() const { return scheme; }
    std::string getHost() const { return host; }
    int getPort() const { return port; }
    std::string getPath() const { return path; }

    // overload the operator == to be able to add URIs to an unordered_set
    inline bool operator==(const URI& other) const {
        return path == other.path;
    }

    UriForm getForm() const {
        if (path[0] == '/') {
            return UriForm::ORIGIN;
        } else if (path[0] == '*') {
            return UriForm::ASTERISK;
        } else if (path.size() >= 13 && path.substr(0, 13) == "hostname:port") {
            return UriForm::AUTHORITY;
        } else if (path.size() >= 7 && path.substr(0, 7) == "http://") {
            return UriForm::ABSOLUTE;
        } else {
            throw std::logic_error("Unexpected URI from Request");
        }
    }

private:
    std::string path;
    std::string scheme;
    std::string host;
    int  port;
};

namespace std {
    template <>
    struct hash<URI> {
        std::size_t operator()(const URI& p) const {
            return std::hash<std::string>()(p.getPath());
        }
    };
}