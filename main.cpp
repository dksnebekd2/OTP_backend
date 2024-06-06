#include <iostream>
#include "json.hpp"
#include "analysis.h"
#include "save_js.h"
#include "openai.hpp"

using json = nlohmann::json;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "프로그램 사용 방법 : " << argv[0] << " <디렉터리 경로>" << std::endl;
        return 1;
    }

    std::string directoryPath = argv[1];
    const char* env_api_key = std::getenv("OPEN_API_KEY");
    std::string apiKey = env_api_key ? std::string(env_api_key) : "";
    if (apiKey.empty()) {
        std::cerr << "API 키가 환경변수에 정상적으로 설정 되어 있지 않아 불러올수 없습니다." << std::endl;
        return 1;
    }

    // OpenAI 초기화
    openai::start(apiKey);

    json results = analyzeFilesInDirectory(directoryPath, [](const std::string& text, const std::string&) -> json {
        return openai::completion().create({
            {"model", "gpt-3.5-turbo"},
            {"prompt", text},
            {"max_tokens", 150}
        });
    });

    saveAnalysisResults("analysis_results.json", results);
    return 0;
}
