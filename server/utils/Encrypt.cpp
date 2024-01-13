#include "Encrypt.hpp"

std::string encrypt(const std::string& plaintext, const std::string& key, const std::string& iv) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr,
                       reinterpret_cast<const unsigned char*>(key.c_str()),
                       reinterpret_cast<const unsigned char*>(iv.c_str()));

    int len;
    unsigned char encrypted_text[plaintext.length() + EVP_MAX_BLOCK_LENGTH];

    int offset = 0;
    for (int chunk_size = 0; chunk_size < plaintext.length(); chunk_size += EVP_MAX_BLOCK_LENGTH) {
        EVP_EncryptUpdate(ctx, encrypted_text + offset, &len,
            reinterpret_cast<const unsigned char*>(plaintext.substr(chunk_size, EVP_MAX_BLOCK_LENGTH).c_str()),
            EVP_MAX_BLOCK_LENGTH);
        offset += len;
    }

    EVP_EncryptFinal_ex(ctx, encrypted_text + offset, &len);

    EVP_CIPHER_CTX_free(ctx);
    // std::string base64_encrypted_text = base64_encode(std::string(reinterpret_cast<char*>(encrypted_text), offset + len));


    return std::string(reinterpret_cast<char*>(encrypted_text), offset + len);
}

Env readKeys(std::string& keyFile) {
    Env env;
    json keyData;
    std::ifstream file(keyFile);
    if(file.is_open()){
        file >> keyData;
        env.key = keyData["Encrypt"]["key"];
        env.iv = keyData["Encrypt"]["iv"];
    } else {
        std::cerr << "Failed to open key file\n";
    }
    file.close();
    return env;
}

void setEnv(Env& env){
    // Retrieve Key and IV
    setenv("ENCRYPTION_KEY", env.key.c_str(),1);
    setenv("ENCRYPTION_IV", env.iv.c_str(), 1);
}
    //Intialize Decryption Context
std::string decrypt_text(std::string& encrypt_text){
    std::string key = std::getenv("ENCRYPTION_KEY");
    std::string iv = std::getenv("ENCRYPTION_IV");

    // Decryption Algorithms and Params
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr,
    (const unsigned char*)key.c_str(),
    (const unsigned char*)iv.c_str());

    //Allocate Buffer for Data
    int out_len1, out_len2;
    unsigned char* plaintext = new unsigned char[encrypt_text.size() + EVP_MAX_BLOCK_LENGTH];

    //Decrypt Data in Chunks
    EVP_DecryptUpdate(ctx, plaintext, &out_len1,
    (const unsigned char*)encrypt_text.c_str(),encrypt_text.size());

    //Finalize Decryption
    EVP_DecryptFinal_ex(ctx, plaintext + out_len1, &out_len2);

    //Constuct Decrypted String
    std::string decrypted_text(reinterpret_cast<char*>(plaintext), out_len1 + out_len2);

    //Clean Up
    delete plaintext;
    EVP_CIPHER_CTX_free(ctx);

    return decrypted_text;
}