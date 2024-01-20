#include "server/utils/AuthenticateUsers.hpp"
#include "server/utils/JsonParser.hpp"
#include "server/utils/Encrypt.hpp"
#include "server/utils/EncryptionKeys.hpp"
#include <algorithm>
#include <crow.h>
#include <fstream>
#include <iostream>
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>


using namespace nlohmann;
std::string seedDataFile = "/home/lavonsampson/c++/BackEnd-Project/data/seedData.json";
std::string userDataFile = "/home/lavonsampson/c++/BackEnd-Project/data/seedUsers.json";
std::string keyDataFile = "/home/lavonsampson/c++/BackEnd-Project/config/keys.json";
int main(int argc, char* argv[])
{
    std::vector<std::string> commands(argv + 1, argv + argc);
    // Main page.
    crow::SimpleApp app;

    // Landing Page for Snippet with string literal for welcome message.
    CROW_ROUTE(app, "/")
    ([&commands]() ->crow::response
     {
        crow::response res;
        if (std::find(commands.begin(), commands.end(), "all") != commands.end()) {
        res.redirect("/snippet");
        return res;
    } else if(std::find(commands.begin(), commands.end(), "users") != commands.end()){
        res.redirect("/users");
        return res;
    } else {
    res.set_header("Content-Type", "text/plain");
        res.write (R"(
 __       __            __                                                      __                       ______                   __           
/  |  _  /  |          /  |                                                    /  |                     /      \                 /  |          
$$ | / \ $$ |  ______  $$ |  _______   ______   _____  ____    ______         _$$ |_     ______        /$$$$$$  |  ______    ____$$ |  ______  
$$ |/$  \$$ | /      \ $$ | /       | /      \ /     \/    \  /      \       / $$   |   /      \       $$ |  $$/  /      \  /    $$ | /      \ 
$$ /$$$  $$ |/$$$$$$  |$$ |/$$$$$$$/ /$$$$$$  |$$$$$$ $$$$  |/$$$$$$  |      $$$$$$/   /$$$$$$  |      $$ |      /$$$$$$  |/$$$$$$$ |/$$$$$$  |
$$ $$/$$ $$ |$$    $$ |$$ |$$ |      $$ |  $$ |$$ | $$ | $$ |$$    $$ |        $$ | __ $$ |  $$ |      $$ |   __ $$ |  $$ |$$ |  $$ |$$    $$ |
$$$$/  $$$$ |$$$$$$$$/ $$ |$$ \_____ $$ \__$$ |$$ | $$ | $$ |$$$$$$$$/         $$ |/  |$$ \__$$ |      $$ \__/  |$$ \__$$ |$$ \__$$ |$$$$$$$$/ 
$$$/    $$$ |$$       |$$ |$$       |$$    $$/ $$ | $$ | $$ |$$       |        $$  $$/ $$    $$/       $$    $$/ $$    $$/ $$    $$ |$$       |
$$/      $$/  $$$$$$$/ $$/  $$$$$$$/  $$$$$$/  $$/  $$/  $$/  $$$$$$$/          $$$$/   $$$$$$/         $$$$$$/   $$$$$$/   $$$$$$$/  $$$$$$$/ 
                                                                                                                                               
                                                                                                                                               
                                                                                                                                               
                                            ______             __                                  __                                          
                                           /      \           /  |                                /  |                                         
                                          /$$$$$$  | _______  $$/   ______    ______    ______   _$$ |_    _______                             
                                          $$ \__$$/ /       \ /  | /      \  /      \  /      \ / $$   |  /       |                            
                                          $$      \ $$$$$$$  |$$ |/$$$$$$  |/$$$$$$  |/$$$$$$  |$$$$$$/  /$$$$$$$/                             
                                           $$$$$$  |$$ |  $$ |$$ |$$ |  $$ |$$ |  $$ |$$    $$ |  $$ | __$$      \                             
                                          /  \__$$ |$$ |  $$ |$$ |$$ |__$$ |$$ |__$$ |$$$$$$$$/   $$ |/  |$$$$$$  |                            
                                          $$    $$/ $$ |  $$ |$$ |$$    $$/ $$    $$/ $$       |  $$  $$//     $$/                             
                                           $$$$$$/  $$/   $$/ $$/ $$$$$$$/  $$$$$$$/   $$$$$$$/    $$$$/ $$$$$$$/                              
                                                                  $$ |      $$ |                                                               
                                                                  $$ |      $$ |                                                               
                                                                  $$/       $$/                                                                
)");
        return res;
    }
     });

    // Get All Code Snippets
    CROW_ROUTE(app, "/snippet")
        .methods("GET"_method)([&commands](const crow::request &req, crow::response &res){
        if (std::find(commands.begin(), commands.end(), "all") != commands.end()) {
            json jsonData = ReadFile(seedDataFile);
            std::ifstream seedData(seedDataFile);

            res.set_header("Content-Type", "application/json");
            res.write(jsonData.dump(4));
            res.end(); 
        } else{
            res.set_header("Content-Type", "application/json");
            res.write("Couldn't find Snippets");
        }
        });

    // Get by ID
    CROW_ROUTE(app, "/snippet/<int>")
        .methods("GET"_method)([](const crow::request &req, crow::response &res, int codeID) {
        json oneSnip = ReadFile(seedDataFile);

        bool found = false;
        for (auto& code : oneSnip) {
        if (codeID == code.at("id")) {
            // Decryption
            std::string encoded_code = code.at("code");
            CROW_LOG_INFO << "Original code: " << encoded_code;
            std::string encrypted_data = base64_decode(encoded_code);
            CROW_LOG_INFO << "Base64 decode: " << encrypted_data;

            std::string decrypted_code = decrypt(encrypted_data, encryption_key, encryption_iv);
            CROW_LOG_INFO <<"Decrypted code" << decrypted_code;
            code.update({{"code", decrypted_code}});
        res.set_header("Content-Type", "application/json");
        res.code = 200;
        res.write(code.dump(4));
        found = true;
        break;
        }
    }

    if (!found) {
    res.code = 404;
    res.write("No Content Found");
    }   
        res.end(); });

    // POST method
    CROW_ROUTE(app, "/snippet")
        .methods("POST"_method)([](const crow::request &req, crow::response &res)
                                {
        json request = request.parse(req.body);
        std::string language = request["language"];
        std::string code = request["code"];

        const std::string encryption_key;
        const std::string encryption_iv;

        // Encrypt code (handles binary input and output)
        std::string encrypt_code = encrypt(code);

        json postData = ReadFile(seedDataFile);
        // Find the highest existing ID
        int maxId = 0;
        for (const auto &snipID : postData){
            int snippetId = snipID["id"];
            maxId = std::max(maxId, snippetId);
        }

        int newId = maxId +1;
        json newSnippet = {
            {"id", newId},
            {"language", language},
            {"code", encrypt_code}
        };
        CROW_LOG_INFO << encrypt_code;
        postData.push_back(newSnippet);

        std::ofstream outputFile(seedDataFile);
        if (outputFile.is_open()) {
            outputFile << postData.dump(4);
            outputFile.close();
            res.code = 201;
            res.write("Snippet created successfully");
        } else {
            res.code = 500;
            res.write("Failed to access data file");
        }
        res.end(); });

    // User Creation
    CROW_ROUTE(app, "/users")
    .methods("POST"_method)([](const crow::request &req, crow::response &res) {
        CROW_LOG_INFO << "Received POST request to /users";

        res.set_header("Content-Type", "application/json");

        // Parse request body
        try {
            json request = request.parse(req.body);
            // int id = request["id"];
            std::string username = request["username"];
            std::string password = request["password"];

            // Generate salt and hash password
            std::string salt = GenerateSalt();
            std::string hashedPassword = HashPassword(password, salt);

            // Log salt and hashed password
            CROW_LOG_INFO << "Generated salt: " << salt;
            CROW_LOG_INFO << "Hashed password: " << hashedPassword;

            
            json postUser = ReadFile(userDataFile);
            // Find the highest existing ID
            int maxId = 0;
            for (const auto &loginID : postUser){
            int userId = loginID["id"];
            maxId = std::max(maxId, userId);
            }

            int new_userID = maxId +1;
            // Create JSON object directly (no user object)
            json userData = {
                {"id", new_userID},
                {"username", username},
                {"hashedPassword", hashedPassword}
            };
            postUser.push_back(userData);

            // Write JSON object to file
            std::ofstream outputFile(userDataFile);
            if (outputFile.is_open()) {
                outputFile << postUser.dump(4) << std::endl;  // Add newline
                outputFile.close();
                CROW_LOG_INFO << "User data written to file successfully";
                res.code = 201;
                res.write("User created successfully");
            } else {
                CROW_LOG_ERROR << "Failed to open file for writing";
                res.code = 500;
                res.write("Failed to write user data to file");
            }
        } catch (const std::exception& e) {
            CROW_LOG_ERROR << "Error parsing request body: " << e.what();
            res.code = 400;
            res.write("Invalid request body");
        }
        res.end();
    });

    // Get All Users.
    CROW_ROUTE(app, "/users")
        .methods("GET"_method)([&commands](const crow::request &req, crow::response &res){
        if (std::find(commands.begin(), commands.end(), "users") != commands.end()) {
            json jsonData = ReadFile(userDataFile);
            std::ifstream seedData(userDataFile);

            res.set_header("Content-Type", "application/json");
            res.write(jsonData.dump(4));
            res.end();
        } else{
            res.set_header("Content-Type", "application/json");
            res.write("Couldn't find Users");
        }
        });

    app.port(8080)
        .multithreaded()
        .run();

    return 0;
}