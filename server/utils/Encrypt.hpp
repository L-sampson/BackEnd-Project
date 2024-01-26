#ifndef SERVER_UTILS_ENCRYPT_HPP_
#define SERVER_UTILS_ENCRYPT_HPP_

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string>
#include <vector>

struct Env {
    std::vector<unsigned char> key;
    std::vector<unsigned char> iv;
};
using namespace nlohmann;

std::string base64_encode(const unsigned char* data, int data_len);
std::string encrypt(const std::string& plaintext);
std::string base64_decode(const std::string& encoded_data);
std::string decrypt(const std::string& cipher64, const std::string& key, const std::string& iv);

#endif // SERVER_UTILS_ENCRYPT_HPP_