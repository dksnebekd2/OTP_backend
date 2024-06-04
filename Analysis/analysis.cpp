#include <fstream>
#include <filesystem>
#include <iostream>
#include "nlohmann/json.hpp"
#include "openai.hpp"

// 파일 시스템 네임스페이스 사용 설정
namespace fs = std::filesystem;
using json = nlohmann::json;

// 주어진 경로의 파일 내용을 읽어 문자열로 반환하는 함수
std::string readFileContents(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "파일을 열 수 없습니다: " << path << std::endl;
        return "";
    }
    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

// 디렉토리 내의 특정 파일을 분석하고 그 결과를 JSON 객체로 반환하는 함수
json analyzeFilesInDirectory(const std::string& directoryPath, const std::function<json(const std::string&, const std::string&)>& analyzeText) {
    // 분석 대상 파일 목록
    std::vector<std::string> relevantFiles = {"LICENSE", "README.MD"};
    json resultJson;
    const char* env_api_key = std::getenv("OPENAI_API_KEY");

    if (!env_api_key) 
    {
        std::cerr << "API 키가 설정되지 않았습니다." << std::endl;
        return resultJson;
    }
    std::string api_key(env_api_key);

    //OpenAI 인스턴스 시작 부분
    auto& openai = openai::start(api_key);

    for (const auto& entry : fs::recursive_directory_iterator(directoryPath)) {
        try {
            // 정규 파일인지 확인
            if (entry.is_regular_file()) {
                std::string filename = entry.path().filename().string();
                // 분석 대상 파일인 경우
                if (std::find(relevantFiles.begin(), relevantFiles.end(), filename) != relevantFiles.end() ||
                    entry.path().extension() == ".cpp") {
                    std::string fileContents = readFileContents(entry.path().string());
                    if (!fileContents.empty()) {
                        // 파일 내용을 분석하고 결과를 JSON 객체에 추가
                        nlohmann::json request = 
                        {
                            {"model", "text-davinci-003"},
                            {"prompt", "Analyze the following code and determine its license:\n\n" + fileContents},
                            {"max_tokens", 100} // 토큰 설정
                        };
                        json analysisResult = analyzeTextWithGPT3(fileContents, apikey);
                        resultJson[filename] = analysisResult;
                    }
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "파일 처리 중 오류 발생 " << entry.path() << ": " << e.what() << std::endl;
        }
    }

    return resultJson;
}
