#include "HttpRequest.h"
#include "StringProcessor.h"

#include <stdexcept>
#include <sstream>

std::string to_string(HttpMethod method) {
    switch (method) {
    case HttpMethod::GET:
        return "GET";
    case HttpMethod::HEAD:
        return "HEAD";
    case HttpMethod::POST:
        return "POST";
    case HttpMethod::PUT:
        return "PUT";
    case HttpMethod::DELETE:
        return "DELETE";
    case HttpMethod::CONNECT:
        return "CONNECT";
    case HttpMethod::OPTIONS:
        return "OPTIONS";
    case HttpMethod::TRACE:
        return "TRACE";
    case HttpMethod::PATCH:
        return "PATCH";
    default:
        return "";
    }
}

HttpMethod string_to_method(const std::string &requestString) {
    std::string requestStringUpper = requestString;
    toUpper(requestStringUpper);
    if (requestStringUpper == "GET") {
        return HttpMethod::GET;
    } else if (requestStringUpper == "HEAD") {
        return HttpMethod::HEAD;
    } else if (requestStringUpper == "POST") {
        return HttpMethod::POST;
    } else if (requestStringUpper == "PUT") {
        return HttpMethod::PUT;
    } else if (requestStringUpper == "DELETE") {
        return HttpMethod::DELETE;
    } else if (requestStringUpper == "CONNECT") {
        return HttpMethod::CONNECT;
    } else if (requestStringUpper == "OPTIONS") {
        return HttpMethod::OPTIONS;
    } else if (requestStringUpper == "TRACE") {
        return HttpMethod::TRACE;
    } else if (requestStringUpper == "PATCH") {
        return HttpMethod::PATCH;
    } else {
        throw std::invalid_argument("Unexpected HTTP method");
    }
}

std::string to_string(const HttpRequest& request) {
    std::ostringstream requestString;
    requestString << to_string(request.getMethod()) << ' ';
    requestString << request.getUri().getPath() << ' ';
    requestString << to_string(request.getVersion()) << "\r\n"; // Adding CRLF based on format
    for (const auto& header : request.getHeaders()) {
        requestString << header.first << ": " << header.second << "\r\n";
    }
    requestString << "\r\n";
    requestString << request.getContent();

    return requestString.str();
}

HttpRequest string_to_request(const std::string& requestString) {
    HttpRequest request;
    std::string startLine, header, body;
    int start = 0, end = 0;

    // start line parsing
    std::string method, path, version;
    URI uri;

    end = requestString.find("\r\n", start);
    startLine = requestString.substr(start, end - start);
    if (end == std::string::npos) {
        throw std::invalid_argument("Invalid request format");
    }
    std::vector<std::string> startLineParts = stringSplit(startLine, ' ');
    if (startLineParts.size() != 3) {
        throw std::invalid_argument("Invalid start line format");
    }
    method = startLineParts[0], path = startLineParts[1], version = startLineParts[2];
    request.setMethod(string_to_method(method));
    uri.setPath(path); request.setUri(uri);
    if (request.getVersion() != string_to_version(version)) {
        throw std::logic_error("HTTP version not supported");
    }

    // header parsing
    std::string key, value;

    start = end + 2;
    end = requestString.find("\r\n\r\n", start);
    if (end != std::string::npos) {
        header = requestString.substr(start, end - start);
        std::vector<std::string> headerLines = stringSplit(header, "\r\n");
        for (auto& line : headerLines) {
            std::vector<std::string> pair = stringSplit(line, ": ");
            if (pair.size() != 2) {
                throw std::invalid_argument("Invalid Header format");
            }
            request.setHeader(pair[0], pair[1]);
        }
    }

    // body parsing
    start = end + 4;
    end = requestString.size();
    if (start < end) {
        body = requestString.substr(start, end - start);
        request.setContent(body);
    }
}