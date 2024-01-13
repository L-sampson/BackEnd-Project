#include "AuthenticateUsers.hpp"
 #include "JsonParser.hpp"


json ReadFile(const std::string& file_path) {
    std::ifstream file(file_path);
    json data;
    
    try {
        if (file.is_open()){
            file >> data;
        } else {
            std::cerr << "Failed to open Database: " << file_path <<std::endl;
        }
        
        
    } catch(std::exception& e) {
        std::cerr << "Could not open: " <<e.what() <<std::endl;
    }
    file.close();
    return data;
}

