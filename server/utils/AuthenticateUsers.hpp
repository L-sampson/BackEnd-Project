#ifndef SERVER_UTILS_AUTHENTICATE_USERS_HPP_
#define SERVER_UTILS_AUTHENTICATE_USERS_HPP_

#include <iostream>
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <fstream>
#include <sstream>
#include <string>

struct User {
    int id;
    std::string username;
    std::string hashedPassword;
};

std::string GenerateSalt();
std::string HashPassword(const std::string& password, const std::string& saltHex);
User NewUser();


#endif