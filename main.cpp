#include <iostream>
#include "json.hpp"
#include "analysis.h"
#include "save_js.h"

extern json analyzeTextWithGPT3(const std::string& text, const std::string& api_key);
extern json analyzeFilesInDirectory(const std::string& directoryPath, const std::function<json(const std::string&, const std::string&)>& analyzeText);
extern void saveAnalysisResults(const std::string& filePath, const json& analysisResults);

using json = nlohmann::json;

// main.cpp : analysis.cpp + save_js.cpp

int main() {
    std::string directoryPath = "path/to/your/folder";
    std::string apiKey = std::getenv("OPENAI_API_KEY");
    if (apiKey.empty()) {
        std::cerr << "OPENAI_API_KEY environment variable is not set." << std::endl;
        return 1;
    }

    json results = analyzeFilesInDirectory(directoryPath, [&apiKey](const std::string& text, const std::string& dummy) -> json {
    return analyzeTextWithGPT3(text, apiKey);
    });


    saveAnalysisResults("analysis_results.json", results);
    return 0;
}