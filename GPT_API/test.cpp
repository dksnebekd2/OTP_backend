#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>  // getenv를 위해 추가
#include <curl.h>

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

    if (curl) {
        const char* env_api_key = std::getenv("OPENAI_API_KEY");
        if (!env_api_key) {
            std::cerr << "Environment variable OPENAI_API_KEY not set." << std::endl;
            return 1;  // API 키가 없으면 프로그램 종료
        }

        std::string headers_str = "Authorization: Bearer " + std::string(env_api_key);
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, headers_str.c_str());

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/engines/gpt-3.5-turbo/completions");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "{\"prompt\": \"Translate 'hello' to French:\", \"max_tokens\":5}");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            std::cout << "Received result: " << readBuffer << std::endl;
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

    std::cout << "Press enter to exit..." << std::endl;
    std::cin.get();  // 사용자 입력 대기

    return 0;
}
