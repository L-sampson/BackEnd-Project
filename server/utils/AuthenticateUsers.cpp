#include "AuthenticateUsers.hpp"
#include <iomanip>
#include <openssl/sha.h>
#include <nlohmann/json.hpp>


using namespace nlohmann;

std::string GenerateSalt() {
    const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789./";
    std::string salt;
    for (int i = 0; i < 16; ++i) {
        salt += characters[rand() % characters.length()];
    }
    return salt;
}

std::string HashPassword(const std::string& password, const std::string& salt) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256_ctx;
    SHA256_Init(&sha256_ctx);
    SHA256_Update(&sha256_ctx, salt.c_str(), salt.length());
    SHA256_Update(&sha256_ctx, password.c_str(), password.length());
    SHA256_Final(hash, &sha256_ctx);

    // Convert hash to hex string
    std::stringstream hexStream;
    hexStream << std::hex << std::setfill('0');
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        hexStream << std::setw(2) << static_cast<unsigned int>(hash[i]);
    }

    std::string hashedPassword = hexStream.str();
    hashedPassword += "$" + salt;
    return hashedPassword;
}


User NewUser() {
    // Creates new User object.
    User newUser;
    std::cout << "Please enter a unique username\n";
    getline(std::cin, newUser.username);
    std::cout << "Please enter a unique password\n";
    std::string password;
    getline(std::cin, password);
    newUser.id++;

    // Generate salt and hash the password
    std::string salt = GenerateSalt();
    std::string hashedPassword = HashPassword(password, salt);
    newUser.hashedPassword = hashedPassword;
    password = "";
    return newUser;
}

void WriteUserToFile(const User& user, const std::string& filename) {
    json userData = {
        {"id", user.id},
        {"username", user.username},
        {"hashedPassword", user.hashedPassword}
    };
    std::ofstream outputFile(filename);
    if(outputFile.is_open()) {
        outputFile <<userData.dump(4);
        outputFile.close();
        std::cout << "User data written to file: " << filename <<std::endl;
    } else {
        std::cerr <<"Failed to write user data to file: " <<filename <<std::endl;
    }
}
