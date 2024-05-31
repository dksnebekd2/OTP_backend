#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <string>
#include <functional>
#include "json.hpp"

namespace fs = std::filesystem;
using json = nlohmann::json;

// 주어진 경로의 파일 내용을 읽어 문자열로 반환
std::string readFileContents(const std::string& path);

// 디렉토리 내의 파일을 분석하고 분석 결과를 JSON 객체로 반환
json analyzeFilesInDirectory(const std::string& directoryPath, const std::function<json(const std::string&, const std::string&)>& analyzeText);

#endif // ANALYSIS_H
