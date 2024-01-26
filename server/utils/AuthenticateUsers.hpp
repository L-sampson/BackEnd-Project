#ifndef SERVER_UTILS_AUTHENTICATE_USERS_HPP_
#define SERVER_UTILS_AUTHENTICATE_USERS_HPP_

#include <iostream>
#include "server/utils/EncryptionKeys.hpp"
#include <chrono>
#include <jwt-cpp/jwt.h>
#include <picojson/picojson.h>
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <fstream>
#include <sstream>
#include <string>

std::string GenerateSalt();
std::string HashPassword(const std::string& password, const std::string& saltHex);
bool comparePassword (std::string& password, std::string& hashedPassword);
picojson::value create_role_claim(const std::string& role);
std::string generate_jwt_with_role(const std::string& role);
std::string get_token(const std::string& authHeader);
bool verifyToken(const std::string& token, const std::string& secret_key);

#endif