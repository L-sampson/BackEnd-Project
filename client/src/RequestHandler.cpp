#include "RequestHandler.hpp"

namespace backend
{
struct WriteCallbackData {
    std::string response;
};

size_t RequestHandler::WriteCallback(void *contents, size_t size, size_t nmemb, void *userdata) {
    WriteCallbackData *data = static_cast<WriteCallbackData*>(userdata);
    data->response.append(static_cast<char*>(contents), size * nmemb);
    return size * nmemb;
}

    CURLcode RequestHandler::InitCurl()
    {
        CURL *curl = curl_easy_init();
        if (!curl)
        {
            std::cerr << "Failed to intialize curl" << std::endl;
            return CURLE_FAILED_INIT;
        }
        CURLcode res;
        curl_easy_setopt(curl, CURLOPT_URL, "http:://localhost:8080");
        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            std::cerr << "Curl perform failed: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
        return res;
    }
}