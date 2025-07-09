#pragma once

#include "HttpMessage.h"
#include "URI.h"

// HTTP Methods
enum class HttpMethod {
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACE,
    PATCH
};

// Get string value of method
std::string to_string(HttpMethod method);
// Get method from a string
HttpMethod string_to_method(const std::string& methodString);

/**
 * @brief This class represents a HttpRequest 
 * 
 */
class HttpRequest : public HttpMessage {
public:
    // default constructor for Http Request class
    HttpRequest() : method(HttpMethod::GET) {}

    /**
     * @brief Construct a new Http Request object with the provided Http Method
     * 
     * @param method Http Method being provided. Could be GET, POST, ...
     */
    HttpRequest(HttpMethod method) : method(method) {}

    /**
     * @brief Destroy the Http Request object
     * 
     */
    ~HttpRequest() = default;

    // Get and set functions
    void setMethod(HttpMethod method) { this->method = method; };
    void setUri(URI uri) { this->uri = uri; }
    HttpMethod getMethod() const { return method; }
    URI getUri() const { return uri; }

    /**
     * @brief Get request content in format of a string
     * 
     * @param request HttpRequest object
     * @return std::string request content in format of a string
     */
    friend std::string to_string(const HttpRequest& request);

    /**
     * @brief Get a HttpRequest from a string
     * 
     * @param requestString request content in format of a string
     * @return HttpRequest object got from the string
     */
    friend HttpRequest string_to_request(const std::string& requestString);


private:
    HttpMethod method;      // Method of the request (GET, POST, HEAD, ...)
    URI uri;                // URI of the request
};