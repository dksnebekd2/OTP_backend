#include <iostream>
#include <string>
#include <curl.h>
#include "json.hpp"
#include <exception>

using json = nlohmann::json;

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

json analyzeTextWithGPT3(const std::string& text, const std::string& api_key) {
    if (api_key.empty()) {
        std::cerr << "API 키가 환경변수에 정상적으로 설정 되어 있지 않아 불러올수 없습니다." << std::endl;
        return json(); // 빈 json 객체 반환
    }

    std::cout << "Initializing CURL..." << std::endl;
    CURL* curl = curl_easy_init();
    std::string readBuffer;
    if (curl) {
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, ("Authorization: Bearer " + api_key).c_str());

        // 토큰수 제한
        json j;
        j["prompt"] = text;
        j["max_tokens"] = 150;

        curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/engines/gpt-3.5-turbo/completions");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, j.dump().c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // SSL 인증서 검증 설정
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
        curl_easy_setopt(curl, CURLOPT_CAINFO,"C:/Users/kyky4/MyProject/OTP_Project/backend/GPT_API/cacert.pem");

        std::cout << "Performing CURL request..." << std::endl;
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            readBuffer.clear(); // 실패 시 빈 문자열로 초기화
        } else {
            std::cout << "CURL request successful!!" << std::endl;
        } 
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    } else {
        std::cerr << "Failed to initialize CURL." << std::endl;
    }
    return json::parse(readBuffer);
}

int main() {
    std::string text = "OPENAI 연동 테스트";
    const char* env_api_key = std::getenv("OPEN_API_KEY");
    std::string api_key = env_api_key ? std::string(env_api_key) : "";

    if (api_key.empty()) {
        std::cerr << "API key not set. Please set your OPEN_API_KEY environment variable." << std::endl;
        return 1; // 종료 코드 1로 종료
    }

    std::cout << "API key loaded successfully!" << std::endl;
    json result = analyzeTextWithGPT3(text, api_key);
    std::cout << "Analysis Result: " << result.dump(4) << std::endl;
    return 0;
}