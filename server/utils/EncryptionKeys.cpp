#include "EncryptionKeys.hpp"

// Retrive keys from file

std::string getKeys(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::string key;
    std::getline(file, key);

    // Check for potential errors during getline
    if (!file.good()) {
        throw std::runtime_error("Error reading key from file: " + filename);
    }

    return key;
}

std::string keyFile = "/home/lavonsampson/c++/BackEnd-Project/config/key.txt";
std::string ivFile = "/home/lavonsampson/c++/BackEnd-Project/config/iv.txt";
extern const std::string encryption_key = getKeys(keyFile);
extern const std::string encryption_iv = getKeys(ivFile);



     