#include "AuthenticateUsers.hpp"
#include <iomanip>
#include <openssl/sha.h>
#include <nlohmann/json.hpp>

using namespace nlohmann;

std::string GenerateSalt()
{
    std::string salt;
    // seeds rand with 33. AscII chars are 33-126.
    srand(33);
    for (int i = 0; i < 16; ++i)
    {
        // generate random number from seed to 126
        int aciiValue = rand() % 126;
        char character = static_cast<char>(aciiValue);
        salt += character;
    }
    return salt;
}

std::string HashPassword(const std::string &password, const std::string &salt)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256_ctx;
    SHA256_Init(&sha256_ctx);
    SHA256_Update(&sha256_ctx, salt.c_str(), salt.length());
    SHA256_Update(&sha256_ctx, password.c_str(), password.length());
    SHA256_Final(hash, &sha256_ctx);

    // Convert hash to hex string
    std::stringstream hexStream;
    hexStream << std::hex << std::setfill('0');
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
    {
        hexStream << std::setw(2) << static_cast<unsigned int>(hash[i]);
    }

    std::string hashedPassword = hexStream.str();
    hashedPassword += "$" + salt;
    return hashedPassword;
}

// Compare entered password and hashedPassword
bool comparePassword(std::string &password, std::string &hashedPassword)
{
    int saltPos = hashedPassword.find("$");
    std::string saltInput = hashedPassword.substr(saltPos + 1);
    std::string userPass = HashPassword(password, saltInput);
    return userPass == hashedPassword;
}

// JWT Token Claims
picojson::value create_role_claim(const std::string &role)
{
    picojson::object claim;
    claim["role"] = picojson::value(role);
    return picojson::value(claim);
}

std::string generate_jwt_with_role(const std::string &role)
{
    picojson::value role_claim = create_role_claim(role);

    auto claims = jwt::create()
                      .set_issuer("http://sniper.io")
                      .set_subject("admin@sniper.io")
                      .set_issued_at(std::chrono::system_clock::now())
                      .set_expires_at(std::chrono::system_clock::now() + std::chrono::hours(24))
                      .set_payload_claim("role", role_claim);

    return claims.sign(jwt::algorithm::hs256{secret_key})
        .c_str();
}

// Extract header
std::string get_token(const std::string &authHeader)
{
    std::string token = "";
    try
    {
        if (authHeader.size() > 7 && authHeader.find("Bearer") != std::string::npos)
        {
            size_t pos = authHeader.find(" ");
            token = authHeader.substr(pos + 1);

        }
        else
        {
            std::cout << "Authorization is not the correct size or cannot find prefix 'Bearer' in header.\n";
        }
    }
    catch (std::exception &e)
    {
        std::cerr << "Error: " << e.what();
    }
    return token;
}

bool verifyToken(const std::string &token, const std::string &secret_key)
{
    try
    {
        auto verify = jwt::verify()
        .allow_algorithm(jwt::algorithm::hs256(secret_key))
        .with_issuer("http://sniper.io");

        auto decoded = jwt::decode(token);
        verify.verify(decoded);
        return true; // Token is verified
    }
    catch (std::exception &e)
    {
        std::cerr << "Error: " << e.what();
        return false;
    }
}
