#ifndef OPENAI_H
#define OPENAI_H

#include "json.hpp"
using json = nlohmann::json;

json analyzeTextWithGPT3(const std::string& text, const std::string& api_key);

#endif // OPENAI_H