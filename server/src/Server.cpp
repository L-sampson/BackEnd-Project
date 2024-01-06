#include <iostream>
#include <crow.h>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>


std::string seedDataFile = "/home/lavonsampson/c++/BackEndProject/data/seedData.json";
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
    std::ifstream seedData(seedDataFile);

    if(seedData.is_open()){
        seedData >> jsonData;
        seedData.close();
    } else {
        std::cerr << "Failed to open file: seedData.json" << std::endl;
    }
    
        res.set_header("Content-Type", "application/json");
        res.write(jsonData.dump(4));
        res.end(); 
        });

    // Get by ID
    CROW_ROUTE(app, "/snippet/<int>")
        .methods("GET"_method)([](const crow::request &req, crow::response &res, int codeID)
                               {
        std::fstream seedData(seedDataFile);
        if(!seedData.is_open()){
            std::cerr<<"Failed to open file\n";
            res.code = 404;
            res.write("Failed to access data file");
            return;
        }
       json oneSnip;
       seedData >> oneSnip;
       seedData.close();

       bool found = false;
    for (const auto& code : oneSnip) {
        if (codeID == code.at("id")) {
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
        int id = request["id"];
        std::string language = request["language"];
        std::string code = request["code"];

        std::ifstream seedData(seedDataFile);
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

        std::ofstream outputFile(seedDataFile);

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