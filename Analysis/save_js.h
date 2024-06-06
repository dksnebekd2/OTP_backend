#ifndef SAVE_JS_H
#define SAVE_JS_H

#include <string>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

// 분석 결과를 JSON 파일로 저장하는 함수
void saveAnalysisResults(const std::string& filePath, const json& analysisResults);

#endif // SAVE_JS_H
