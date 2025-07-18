#include "HttpResponse.h"

#include <sstream>

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

std::string to_string(const HttpResponse& response, bool send_content) {
  std::ostringstream responseString;

  responseString << to_string(response.getVersion()) << ' ';
  responseString << static_cast<int>(response.getStatusCode()) << ' ';
  responseString << to_string(response.getStatusCode()) << "\r\n";
  for (const auto& p : response.getHeaders())
    responseString << p.first << ": " << p.second << "\r\n";
  responseString << "\r\n";
  if (send_content) responseString << response.getContent();

  return responseString.str();
}