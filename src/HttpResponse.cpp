#include "HttpResponse.h"

std::string to_string(HttpStatusCode statusCode) {
    switch (statusCode) {
        case HttpStatusCode::Ok:
            return "Ok";
        case HttpStatusCode::Continue:
            return "Continue";
        case HttpStatusCode::Found:
            return "Found";
        case HttpStatusCode::NotFound:
            return "Not Found";
        case HttpStatusCode::BadRequest:
            return "Bad Request";
        case HttpStatusCode::Forbidden:
            return "Forbidden";
        case HttpStatusCode::BadGateway:
            return "Bad Gateway";
        case HttpStatusCode::MovedPermanently:
            return "Moved Permanently";
        case HttpStatusCode::MethodNotAllowed:
            return "Method Not Allowed";
        case HttpStatusCode::NoContent:
            return "No Content";
        case HttpStatusCode::ServiceUnavailable:
            return "Service Unavailable";
        case HttpStatusCode::InternalServerError:
            return "Internal Server Error";
        case HttpStatusCode::GatewayTimeout:
            return "Gateway Timeout";
        case HttpStatusCode::NotImplemented:
            return "Not Implemented";
        default:
            return "";
    }
}