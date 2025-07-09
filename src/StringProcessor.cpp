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