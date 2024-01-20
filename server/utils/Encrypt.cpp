#include "Encrypt.hpp"
#include "EncryptionKeys.hpp"

// Encode using base64.
std::string base64_encode(const unsigned char* data, int data_len) {
    // Create a memory buffer BIO for ouput
    BIO *base64 = BIO_new(BIO_f_base64());
    BIO *memory = BIO_new(BIO_s_mem());
    base64 = BIO_push(base64, memory);

    //Encode the data
    BIO_write(base64, data, data_len);
    BIO_flush(base64);

    // Retrieve the encoded data from the memory BIO
    BUF_MEM *bufferPtr;
    BIO_get_mem_ptr(base64, &bufferPtr);
    std::string encoded_data(bufferPtr->data, bufferPtr->length);

    // Free memory
    BIO_free_all(base64);

    return encoded_data;
}

// Encrypt using AES-256
std::string encrypt(const std::string& plaintext) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    try {
        // Initialize encryption context
        if(!EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, (const unsigned char*)encryption_key.c_str(), (const unsigned char*)encryption_iv.c_str())){
            throw std::runtime_error("Failed to intialize encryption context");
        }
        int out_len1, out_len2;
        unsigned char ciphertext[128];
        if (!EVP_EncryptUpdate(ctx, ciphertext, &out_len1, (const unsigned char*)plaintext.c_str(), plaintext.length())) {
            throw std::runtime_error("Encryption update failed");
        }
        if(!EVP_EncryptFinal_ex(ctx, ciphertext + out_len1, &out_len2)){
            throw std::runtime_error("Encryption finalization failed");
        }

        //Base64-encode ciphertext
        std::string base64_ciphertext = base64_encode(ciphertext, out_len1 + out_len2);

        return base64_ciphertext;
    } catch (const std::runtime_error& e) {
        std::cerr << "Encryption error: " <<e.what() <<std::endl;
        EVP_CIPHER_CTX_free(ctx);
        throw;
    }
}

std::string base64_decode(const std::string& encoded_data) {
    BIO *base = BIO_new(BIO_f_base64());
    BIO *b_mem = BIO_new_mem_buf((void *)encoded_data.c_str(), encoded_data.size());
    base = BIO_push(base, b_mem);

    //Igonore new line characters
    BIO_set_flags(base, BIO_FLAGS_BASE64_NO_NL);
    std::string decoded_data;
    char buffer[512];
    int bytes_read;
    while ((bytes_read = BIO_read(base, buffer, sizeof(buffer)))>0){
        decoded_data.append(buffer, bytes_read);
    }

    BIO_free_all(base);
    return decoded_data;
}

std::string decrypt(const std::string& cipher64, const std::string& key, const std::string& iv){
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
if(key.empty() || iv.empty()) {
    throw std::runtime_error("Key or IV not found");
}

    try {
        std::vector<unsigned char> ciphertext(cipher64.begin(), cipher64.end());
        // Initialize decryption context
        if (!EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(),nullptr, (const unsigned char*)encryption_key.c_str(), (const unsigned char*)encryption_iv.c_str())){
            throw std::runtime_error("Failed to intialize decryption");
        }

        int out_len1, out_len2;
        unsigned char plaintext[512];
        if (!EVP_DecryptUpdate(ctx, plaintext, &out_len1, ciphertext.data(), ciphertext.size())) {
            throw std::runtime_error("Decryption update failed");
        }
        if (!EVP_DecryptFinal_ex(ctx, plaintext + out_len1, &out_len2)) {
            throw std::runtime_error("Decryption finalization failed");
        }
        std::string decrypted_text(reinterpret_cast<char*>(plaintext), out_len1 + out_len2);
        return decrypted_text;
    } catch (const std::runtime_error& e) {
        std::cerr << "Decryption error: " << e.what() << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        throw;
    }
}