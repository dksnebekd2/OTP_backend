#include <fstream>
//#include "nlohmann/json.hpp"
#include "save_js.h"

using json = nlohmann::json;

// save_js.cpp : 분석결과를 받아 JSON 파일로 저자하는 기능을 수행한다.

// 분석 결과를 JSON 파일로 저장
// 분석 결과 JSON 객체와 저장할 경로를 인자로 받고, 결과를 파일로 저장한다.
void saveAnalysisResults(const std::string& filePath, const json& analysisResults) {
    std::ofstream outFile(filePath);
    outFile << analysisResults.dump(4);
    outFile.close();
}
