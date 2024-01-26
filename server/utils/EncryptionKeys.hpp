#ifndef CONFIG_ENCRYPTION_KEYS_HPP_
#define CONFIG_ENCRYPTION_KEYS_HPP_
#include <iostream>
#include <fstream>
std::string getKeys(const std::string& filename);
extern const std::string encryption_key;
extern const std::string encryption_iv;
extern const std::string secret_key;


#endif