#pragma once

#include <string>
#include <vector>

/**
 * @brief Turn a string into lowercase
 * 
 * @param str input string
 */
void toLower(std::string& str);

/**
 * @brief Turn a string into uppercase
 * 
 * @param str input string
 */
void toUpper(std::string& str);

/**
 * @brief Split a string with a char delimiter
 * 
 * @param str input string to be splitted
 * @param delimiter delimiter as a char
 * @return std::vector<std::string> vector of strings after splitting
 */
std::vector<std::string> stringSplit(const std::string& str, char delimiter);

/**
 * @brief Split a string with a string delimiter
 * 
 * @param str input string to be splitted
 * @param delimiter delimiter as a string
 * @return std::vector<std::string> vector of strings after splitting
 */
std::vector<std::string> stringSplit(const std::string& str, const std::string& delimiter);