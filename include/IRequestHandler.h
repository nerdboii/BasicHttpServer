#pragma once

#include "HttpResponse.h"
#include "HttpRequest.h"

/**
 * @brief Http request handler interface to avoid coupling
 * 
 */
class IRequestHandler {
public:
    virtual HttpResponse handleRequest(const HttpRequest& request) = 0;
    virtual ~IRequestHandler() = default;
};