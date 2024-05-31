#include <iostream>
#include "json.hpp"
#include "analysis.h"
#include "save_js.h"
#include "openai.h"

// extern json analyzeTextWithGPT3(const std::string& text, const std::string& api_key);
// extern json analyzeFilesInDirectory(const std::string& directoryPath, const std::function<json(const std::string&, const std::string&)>& analyzeText);
// extern void saveAnalysisResults(const std::string& filePath, const json& analysisResults);

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
        std::cerr << "API 키가 정상적으로 입력되지 않았습니다." << std::endl;
        return 1;
    }

    // 디렉토리 내의 파일을 분석하고 결과를 JSON 객체로 반환
    json results = analyzeFilesInDirectory(directoryPath, [&apiKey](const std::string& text, const std::string& unused) -> json {
        return analyzeTextWithGPT3(text, apiKey);
    });

     // 분석 결과를 JSON 파일로 저장
    saveAnalysisResults("analysis_results.json", results);
    return 0;
}