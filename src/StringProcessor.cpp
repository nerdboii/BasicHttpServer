#include "StringProcessor.h"

void toLower(std::string& str) {
    for (auto& c : str) {
        if (c >= 'A' && c <= 'Z')
            c = tolower(c);
    }
}

void toUpper(std::string& str) {
    for (auto& c : str) {
        if (c >= 'a' && c <= 'z')
            c = toupper(c);
    }
}

std::vector<std::string> stringSplit(const std::string& str, char delimiter) {
    std::vector<std::string> result;
    size_t start = 0;
    size_t end;

    while ((end = str.find(delimiter, start)) != std::string::npos) {
        result.push_back(str.substr(start, end - start));
        start = end + 1;
    }

    result.push_back(str.substr(start));
    return result;
}

std::vector<std::string> stringSplit(const std::string& str, const std::string& delimiter) {
    std::vector<std::string> result;
    size_t start = 0;
    size_t end;

    while ((end = str.find(delimiter, start)) != std::string::npos) {
        result.push_back(str.substr(start, end - start));
        start = end + delimiter.length();
    }

    result.push_back(str.substr(start));
    return result;
}

bool ends_with(const std::string& str, const std::string& suffix) {
    if (suffix.size() > str.size()) return false;
    return std::equal(suffix.rbegin(), suffix.rend(), str.rbegin());
}