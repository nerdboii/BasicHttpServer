#pragma once

#include <string>
#include <map>

// HTTP Version, i'm only supporting HTTP/1.1
enum class HttpVersion {
    HTTP_1_0 = 10,
    HTTP_1_1 = 11
};

// Get string value of version
std::string to_string(HttpVersion version);

// Get version from a string
HttpVersion string_to_version(const std::string& versionString);

/**
 * @brief A http message interface for request and response
 * 
 * This class sets up a Http message, containing version, headers and content
 */
class HttpMessage {
public:
    /**
     * @brief Constructs a new HttpMessage with default HTTP version (HTTP/1.1).
     */
    HttpMessage() : version(HttpVersion::HTTP_1_1) {}

    /**
     * @brief Virtual destructor for HttpMessage.
     */
    virtual ~HttpMessage() = default;

    /**
     * @brief Get the Header Value of the provided key
     * 
     * @param key provided key
     * @return std::string value mapping with key
     */
    std::string getHeaderValueAt(const std::string& key) const {
        if (headers.count(key) > 0) return headers.at(key);
        return "";
    }

    // Get the map of keys and values
    std::map<std::string, std::string> getHeaders() const {
        return headers;
    }

    // Get and set functions
    HttpVersion getVersion() const { return version; }
    void setVersion(HttpVersion ver) { version = ver; }
    int getContentLen() const { return content.size(); }
    std::string getContent() const { return content; }
    void setContent(const std::string& body) { 
        content = body;
        setContentLen();
    }
    void setContentLen() {
        setHeader("Content-Length", std::to_string(content.length()));
    }

    /**
     * @brief Set or update a header key-value pair.
     * 
     * @param key The name of the header.
     * @param value The value to associate with the header.
     */
    void setHeader(const std::string& key, const std::string& value) {
        headers[key] = std::move(value);
    }

    /**
     * @brief Remove a header by its key.
     * 
     * @param key The name of the header to remove.
     */
    void removeHeader(const std::string& key) {
        headers.erase(key);
    }

    /**
     * @brief Remove all headers from the message.
     */
    void clearHeader() {
        headers.clear();
    }

protected:
    HttpVersion version;                        // HTTP version used in this message
    std::map<std::string, std::string> headers; // HTTP headers keys and values
    std::string content;                        // Message body content
};