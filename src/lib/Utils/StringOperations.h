#pragma once

#include <algorithm>
#include <string>

inline std::string removeWhitespace(const std::string& str)
{
    std::string result = str;
    result.erase(std::remove_if(result.begin(), result.end(), [](unsigned char c) { return std::isspace(c); }), result.end());
    return result;
}