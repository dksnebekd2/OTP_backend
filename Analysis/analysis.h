#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <string>
#include <functional>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

// 주어진 경로의 파일 내용을 읽어 문자열로 반환하는 함수
std::string readFileContents(const std::string& path);

// 디렉토리 내의 특정 파일을 분석하고 그 결과를 JSON 객체로 반환하는 함수
json analyzeFilesInDirectory(const std::string& directoryPath, const std::function<json(const std::string&, const std::string&)>& analyzeText);

#endif // ANALYSIS_H