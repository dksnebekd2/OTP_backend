#include <iostream>
#include "nlohmann/json.hpp"
#include "analysis.h"
#include "save_js.h"
#include "openai.hpp"

using json = nlohmann::json;

// main.cpp : analysis.cpp + save_js.cpp

int main(int argc, char* argv[]) {
    // 하드 코딩 방식 (C++ 프로그램화 해야 하기 때문에 사용하지 못함.)
    // std::string directoryPath = "path/to/your/folder";

    if (argc < 2) 
    {
        std::cerr << "프로그램 사용 방법 : " << argv[0] << " < 디렉터리 경로 >" << std::endl;
        return 1;
    }

    std::string directoryPath = argv[1];
    std::string apiKey = std::getenv("OPENAI_API_KEY");
    if (apiKey.empty()) {
        std::cerr << "API 키가 환경변수에 설정되어 있지 않습니다." << std::endl;
        return 1;
    }
    std::string api_key(env_api_key);

    // 디렉토리 내의 파일을 분석하고 결과를 JSON 객체로 반환
    json results = analyzeFilesInDirectory(directoryPath, [&api_key](const std::string& text) -> json {
        nlohmann::json request = {
            {"model", "text-davinci-003"},
            {"prompt", "Analyze the following code and determine its license:\n\n" + text},
            {"max_tokens", 150}
        };
        return openai::completion().create(request);
    });

    // 분석 결과를 JSON 파일로 저장
    saveAnalysisResults("analysis_results.json", results);
    return 0;
}