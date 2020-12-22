#pragma once

#include <cctype>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

inline std::string toUpper(const std::string& s)
{
    std::string ret = s;
    std::transform(s.begin(), s.end(), ret.begin(), [](unsigned char c) { return std::toupper(c); });
    return ret;
}

inline std::string toLower(const std::string& s)
{
    std::string ret = s;
    std::transform(s.begin(), s.end(), ret.begin(), [](unsigned char c) { return std::tolower(c); });
    return ret;
}

static const char* ws = " \t\n\r\f\v";

// trim from end of string (right)
inline void rtrim(std::string& s, const char* t = ws)
{
    s.erase(s.find_last_not_of(t) + 1);
}

// trim from beginning of string (left)
inline void ltrim(std::string& s, const char* t = ws)
{
    s.erase(0, s.find_first_not_of(t));
}

// trim from both ends of string (right then left)
inline void trim(std::string& s, const char* t = ws)
{
    rtrim(s, t);
    ltrim(s, t);
}

inline std::vector<std::string> split(std::string s, char delim) {

    trim(s);

    std::vector<std::string> elems;
    std::istringstream iss(s);
    std::string item;
    
    while (std::getline(iss, item, delim)) {
        trim(item);
        if (item != "")
            elems.emplace_back(item);
    }

    return elems;
}