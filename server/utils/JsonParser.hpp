#ifndef SERVER_UTILS_JSON_PARSER_HPP_
#define SERVER_UTILS_JSON_PARSER_HPP_
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using namespace nlohmann;
json ReadFile(const std::string& file_path);

#endif // SERVER_UTILS_JSON_PARSER_HPP_ 