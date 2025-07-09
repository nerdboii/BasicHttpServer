#pragma once

#include "HttpMessage.h"

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
    NotImplemented = 501,
    BadGateway = 502,
    ServiceUnavailable = 503,
    GatewayTimeout = 504
};

// Get string value of status code
std::string to_string(HttpStatusCode statusCode);

/**
 * @brief This class represents a Http Response
 * 
 */
class HttpResponse : public HttpMessage {
public:
    /**
     * @brief Construct a new Http Response object with status code: Ok
     * 
     */
    HttpResponse() : statusCode(HttpStatusCode::Ok) {}

    /**
     * @brief Construct a new Http Response object with provided status code
     * 
     * @param code 
     */
    HttpResponse(HttpStatusCode code) : statusCode(code) {}

    /**
     * @brief Destroy the Http Response object
     * 
     */
    ~HttpResponse() = default;

    // Get and Set functions
    void SetStatusCode(HttpStatusCode code) { statusCode = code; }
    HttpStatusCode getStatusCode() const { return statusCode; }

    /**
     * @brief Get response content in the format of a string
     * 
     * @param request The request to get content from
     * @param sendBody A control flag which decides whether or not the response body should be included
     * @return std::string Content of response in the format of a string
     */
    friend std::string to_string(const HttpResponse& request, bool sendBody);

    /**
     * @brief Get a HttpResponse from a string
     * 
     * @param responseString Response content in format of a string
     * @return HttpResponse object got from the string
     */
    friend HttpResponse string_to_response(const std::string& responseString);

private:
    HttpStatusCode statusCode;      // Status code of a request (100, 101, 200, ...)
};