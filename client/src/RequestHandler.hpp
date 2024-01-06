#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP

#include <iostream>
#include <curl/curl.h>
#include <string>

namespace backend
{
    class RequestHandler
    {
    public:
        CURLcode InitCurl();

    private:
        size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userdata);
        CURL *curl;
    };
} // namespace backend

#endif