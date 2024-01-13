#ifndef SERVER_UTILS_ENCRYPT_HPP_
#define SERVER_UTILS_ENCRYPT_HPP_

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <openssl/evp.h>
#include <string>

struct Env {
    std::string key;
    std::string iv;
};
using namespace nlohmann;

std::string encrypt(const std::string& plaintext, const std::string& key, const std::string& iv);
Env readKeys(std::string& keyFile);
void setEnv(Env& env);
std::string decrypt_text(std::string& encrypt_text);

#endif // SERVER_UTILS_ENCRYPT_HPP_