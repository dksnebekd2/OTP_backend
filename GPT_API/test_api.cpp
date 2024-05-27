#include <curl.h>
#include <iostream>
#include <string>
#include "json.hpp"

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main() {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/engines/gpt-3.5-turbo/completions");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // 환경 변수에서 API 키 읽어오기
        const char* api_key = std::getenv("OPENAI_API_KEY");
        if (!api_key) {
            std::cerr << "API key is not set in the environment variables." << std::endl;
            return 1;
        }

        struct curl_slist* headers = NULL;
        std::string auth_header = "Authorization: Bearer " + std::string(api_key);
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, auth_header.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            std::cout << "HTTP Response Code: " << res << std::endl;
            std::cout << "Received result: " << readBuffer << std::endl;
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }

    curl_global_cleanup();
    return 0;
}
