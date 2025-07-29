#pragma once

#include "HttpResponse.h"
#include "HttpRequest.h"
#include <unordered_map>

const std::unordered_map<std::string, std::pair<std::string, std::string>> fileExtensions = {
    {".html", {"/html", "text/html"}},
    {".json", {"/json", "application/json"}},
    {".txt" , {"/txt" , "text/plain"}},
    {".png" , {"/img" , "image"}}
};

/**
 * @brief Http request handler interface to avoid coupling
 * 
 */
class IRequestHandler {
public:
    virtual HttpResponse handleRequest(const HttpRequest& request) = 0;
    virtual ~IRequestHandler() = default;

    /**
    * @brief parsing a request URI
    * 
    * @param filePath URI file path
    * @return std::pair<std::string, std::string> : first -> extension of resource, second -> content type
    */
    std::pair<std::string, std::string> parseFilePath(std::string filePath) {
        for (const auto& extension : fileExtensions) {
            if (ends_with(filePath, extension.first)) {
                return extension.second;
            }
        }
        
        // There's no satisfied resource
        throw std::invalid_argument("Not found!");
    }
};