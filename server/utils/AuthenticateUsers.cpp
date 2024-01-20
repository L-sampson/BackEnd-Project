#include "AuthenticateUsers.hpp"
#include <iomanip>
#include <openssl/sha.h>
#include <nlohmann/json.hpp>

using namespace nlohmann;

std::string GenerateSalt() {
    std::string salt;
    // seeds rand with 33. AscII chars are 33-126.
    srand(33);
    for (int i = 0; i < 16; ++i) {
        // generate random number from seed to 126
        int aciiValue = rand() % 126;
        char character = static_cast<char>(aciiValue);
        salt += character;
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

// Compare entered password and hashedPassword
bool comparePassword (std::string& password, std::string& hashedPassword){
    int saltPos = hashedPassword.find("$");
    std::string saltInput = hashedPassword.substr(saltPos + 1);
    std::string userPass = HashPassword(password, saltInput);
    return userPass == hashedPassword;
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

// JWT Token Claims
picojson::value create_role_claim(const std::string& role) {
    picojson::object claim;
    claim["role"] = picojson::value(role);
    return picojson::value(claim);
}

std::string generate_jwt_with_role(const std::string& role) {
    picojson::value role_claim = create_role_claim(role);

    auto claims = jwt::create()
        .set_issuer("http://sniper.io")
        .set_subject("admin@sniper.io")
        .set_issued_at(std::chrono::system_clock::now())
        .set_expires_at(std::chrono::system_clock::now() + std::chrono::hours(24))
        .set_payload_claim("role", role_claim);

    return claims.sign(jwt::algorithm::hs256{token})
                 .c_str();
}

bool validate_JWT(const std::string& token) {
    try {
        jwt::decoded_jwt<jwt::traits::kazuho_picojson> decoded = jwt::decode(token);
        const jwt::header<jwt::traits::kazuho_picojson> &header = decoded.get_header();
        const jwt::claim<jwt::traits::kazuho_picojson>& payload = decoded.get_payload();
    } catch(const std::exception& e) {
        return false;
    }
}
