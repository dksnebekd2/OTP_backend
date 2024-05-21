#include <cpr/cpr.h>
#include <iostream>

int main() {
    std::string api_key = "YOUR_API_KEY_HERE";  // 여기에 OpenAI API 키를 입력.
    std::string prompt = "Translate the following English text to French: 'Hello, how are you?'";

    cpr::Response r = cpr::Post(
        cpr::Url{"https://api.openai.com/v1/engines/davinci/completions"},
        cpr::Header{{"Authorization", "Bearer " + api_key},
                    {"Content-Type", "application/json"}},
        cpr::Body{R"({"prompt": ")" + prompt + R"(", "max_tokens": 60})"},
        cpr::Timeout{10000}  // 10 seconds timeout for the request
    );

    if (r.status_code == 200) {
        std::cout << "Response from OpenAI: " << r.text << std::endl;
    } else {
        std::cout << "Failed to make a request: " << r.status_code << std::endl;
    }

    return 0;
}
