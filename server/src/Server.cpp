#include <iostream>
#include <crow.h>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>

using namespace nlohmann;

int main()
{
    // Main page.
    crow::SimpleApp app;

    // Landing Page for Snippet with string literal for welcome message.
    CROW_ROUTE(app, "/")
    ([]()
     {
        std::string welcome = R"(
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
)";
        return welcome; });

    // Get All Code Snippets
    CROW_ROUTE(app, "/snippet")
        .methods("GET"_method)([](const crow::request &req, crow::response &res){
        json jsonData;
    std::ifstream seedData("../../data/seedData.json");
    if(seedData.is_open()){
        seedData >> jsonData;
        seedData.close();
    }
    
        res.set_header("Content-Type", "application/json");
        // std::string jString = jsonData.dump(4);
        res.write(jsonData.dump(4));
        res.end(); 
        });

    // Get by ID
    //  Bug Return status should be not found If ID doesn't exist.
    CROW_ROUTE(app, "/snippet/<int>")
        .methods("GET"_method)([](const crow::request &req, crow::response &res, int codeID)
                               {
        std::fstream seedData("../../data/seedData.json");
        if(!seedData.is_open()){
            std::cerr<<"Failed to open file\n";
            res.code = 500;
            res.write("Failed to access data file");
            return;
        }
       json oneSnip;
       seedData >> oneSnip;
       seedData.close();

       bool found = false;
    for (const auto& code : oneSnip) {
        if (codeID == code.at("id")) {
        std::string oneCodeLang = code.dump(4);
        res.code = 200;
        res.write(oneCodeLang);
        found = true;
        break;
        }
    }

    if (!found) {
    res.code = 204;
    res.write("No Content Found");
    }   
            
        res.end(); });

    // POST method
    CROW_ROUTE(app, "/snippet")
        .methods("POST"_method)([](const crow::request &req, crow::response &res)
                                {
        json request = request.parse(req.body);
        int id = request["id"];
        std::string language = request["language"];
        std::string code = request["code"];

        std::ifstream seedData("../../data/seedData.json");
        if (!seedData.is_open()){
            std::cerr << "Failed to open file\n";
            res.code = 500;
            res.write("Failed to access data file");
            return;
        }
        json currentData;
        seedData >> currentData;
        seedData.close();
        json newSnippet = {
            {"id", id},
            {"language", language},
            {"code", code}
        };

        currentData.push_back(newSnippet);

        std::ofstream outputFile("../../data/seedData.json");

        if (outputFile.is_open()) {
            outputFile << currentData.dump(4);
            outputFile.close();
            res.code = 201;
            res.write("Snippet created successfully");
        } else {
            res.code = 500;
            res.write("Failed to access data file");
        }

        res.end(); });

    app.port(8080)
        .multithreaded()
        .run();

    return 0;
}