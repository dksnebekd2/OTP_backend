#ifndef SAVE_JS_H
#define SAVE_JS_H

#include <string>
#include "json.hpp"

using json = nlohmann::json;

// 분석 결과를 JSON 파일로 저장
// 분석 결과 JSON 객체와 저장할 경로를 인자로 받고, 결과를 파일로 저장한다.
void saveAnalysisResults(const std::string& filePath, const json& analysisResults);

#endif // SAVE_JS_H