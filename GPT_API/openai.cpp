#include <iostream>
#include <string>
#include <curl.h>
#include "json.hpp"

using json = nlohmann::json;

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

json analyzeTextWithGPT3(const std::string& text) {
    std::string api_key = std::getenv("OPEN_API_KEY"); // 환경 변수에서 API 키 가져오기
    if (api_key.empty()) {
        std::cerr << "API key not set in OPEN_API_KEY environment variable." << std::endl;
        return json(); // 빈 json 객체 반환
    }

    CURL* curl = curl_easy_init();
    std::string readBuffer;
    if (curl) {
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, ("Authorization: Bearer " + api_key).c_str());

        //토큰수 제한 
        json j;
        j["prompt"] = text;
        j["max_tokens"] = 150; 

        curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/engines/gpt-3.5-turbo/completions");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, j.dump().c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            readBuffer.clear(); // 실패 시 빈 문자열로 초기화
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
    return json::parse(readBuffer);
}