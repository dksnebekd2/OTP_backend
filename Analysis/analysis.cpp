#include <fstream>
#include <filesystem>
#include <iostream>
#include "json.hpp"

namespace fs = std::filesystem;
using json = nlohmann::json;

std::string readFileContents(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "파일을 열 수 없습니다: " << path << std::endl;
        return "";
    }
    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

json analyzeFilesInDirectory(const std::string& directoryPath, const std::function<json(const std::string&, const std::string&)>& analyzeText) {
    std::vector<std::string> relevantFiles = {"LICENSE", "README.MD"};
    json resultJson;

    for (const auto& entry : fs::recursive_directory_iterator(directoryPath)) {
        try {
            if (entry.is_regular_file()) {
                std::string filename = entry.path().filename().string();
                if (std::find(relevantFiles.begin(), relevantFiles.end(), filename) != relevantFiles.end() ||
                    entry.path().extension() == ".cpp") {
                    std::string fileContents = readFileContents(entry.path().string());
                    if (!fileContents.empty()) {
                        json analysisResult = analyzeText(fileContents, "");
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
