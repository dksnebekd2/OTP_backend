#include <fstream>
#include <filesystem>
#include "json.hpp"

// analysis.cpp : 사용자의 디렉토리에서 특정 파일을 찾아 내용을 읽고
//                분석 요청을 보내는 기능 수행.

namespace fs = std::filesystem;
using json = nlohmann::json;

// 주어진 경로의 파일 내용을 읽어 문자열로 반환
std::string readFileContents(const std::string& path) {
    std::ifstream file(path);
    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

// 디렉토리 내의 파일을 분석하고 분석 결과를 JSON 객체로 반환
json analyzeFilesInDirectory(const std::string& directoryPath, const std::function<json(const std::string&, const std::string&)>& analyzeText) {
    std::vector<std::string> relevantFiles = {"LICENSE", "README.MD"};
    json resultJson;

    for (const auto& entry : fs::recursive_directory_iterator(directoryPath)) {
        if (entry.is_regular_file()) {
            std::string filename = entry.path().filename().string();
            if (std::find(relevantFiles.begin(), relevantFiles.end(), filename) != relevantFiles.end() ||
                entry.path().extension() == ".cpp") {
                std::string fileContents = readFileContents(entry.path().string());
                json analysisResult = analyzeText(fileContents, "YOUR_API_KEY_HERE");
                resultJson[filename] = analysisResult;
            }
        }
    }

    return resultJson;
}
