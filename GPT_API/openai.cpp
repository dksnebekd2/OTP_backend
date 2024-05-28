#include <string>
#include <curl.h>
#include "json.hpp"

using json = nlohmann::json;

// CURL 콜백 함수, HTTP 응답 데이터를 문자열로 저장
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// 텍스트를 분석하기 위해 OpenAI GPT-3.5 API를 호출하는 함수
json analyzeTextWithGPT3(const std::string& text, const std::string& api_key) {
    CURL* curl = curl_easy_init();
    std::string readBuffer;
    if (curl) {
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, ("Authorization: Bearer " + api_key).c_str());

        json j;
        j["prompt"] = text;
        j["max_tokens"] = 150;  // 분석할 토큰의 최대 수

        curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/engines/gpt-3.5-turbo/completions");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, j.dump().c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
    return json::parse(readBuffer);
}
