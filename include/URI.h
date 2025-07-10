#pragma once

#include "StringProcessor.h"

#include <string>

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