#pragma once

#include <string>
#include <map>

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

// HTTP Version, i'm only supporting HTTP/1.1
enum class HttpVersion {
    HTTP_1_0 = 10,
    HTTP_1_1 = 11
};

// HTTP response status:
enum class HttpStatusCode {
    Continue = 100,
    SwitchProtocol = 101,
    Ok = 200,
    Created = 201,
    NoContent = 204,
    MovedPermanently = 301,
    Found = 302,
    NotModified = 304,
    BadRequest = 400,
    Unauthorized = 401,
    Forbidden = 403,
    NotFound = 404,
    MethodNotAllowed = 405,
    InternalServerError = 500,
    BadGateway = 502,
    ServiceUnavailable = 503,
    GatewayTimeout = 504
};

class HttpMessage {
protected:
    HttpVersion version;
    std::map<std::string, std::string> headers;
    std::string content;
};