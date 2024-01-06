# Multiverse BackEnd Project.
## TechStack:
1. **C++ STL fstream** 
    - Open Seed Data Files.
2. **Curl Library** 
    - Client-Side API/HTTP Request(GET, POST, PUT, DELETE)
3. **Crow Library**
    - Server-Side API/HTTP
3. **Modern C++ Json(Nlohmann)** 
    - Parse JSON Objects
4. **GTest**
    - Testing Framework
4. **Protobuf(Potential Stack)** 
    - Create Messages to Serialize & Deserialize Data
5. **OpenSSL**
    - Encryption for password and username
6. **Botan**
    - Encryption

### FileTree Structure:
```
project/
    |-- client/
    |   |-- src/
    |   |   |-- main.cpp             // Main function for the client-side application
    |   |   |-- RequestHandler.cpp   // Implementation of client-side HTTP request methods
    |   |   |-- RequestHandler.h     // Header file for the client-side RequestHandler class
    |   |   |-- JSONParser.cpp       // JSON parsing logic for client-side
    |   |   |-- JSONParser.h         // Header file for the JSONParser class used by the client
    |   |-- tests/                   // Client-side unit tests (if applicable)
    |       |-- test_request.cpp     // Unit test file for client-side HTTP requests (if applicable)
    |
    |-- server/
    |   |-- src/
    |   |   |-- main.cpp             // Main function for the server-side application
    |   |   |-- Server.cpp           // Server implementation using Crow
    |   |   |-- Server.h             // Header file for the Server class
    |   |   |-- JSONParser.cpp       // JSON parsing logic for server-side
    |   |   |-- JSONParser.h         // Header file for the JSONParser class used by the server
    |   |-- tests/                   // Server-side unit tests (if applicable)
    |       |-- test_routes.cpp      // Unit test file for server-side route handling (if applicable)
    |
    |-- data/
    |   |-- seed.json                // Seed data file
    |
    |-- README.md                    // Project documentation
    |-- Bazel/CMake BuildFile        // Build automation script
```

## Week 2 MVP:
### User Stories
#### Encryption
- As a user, I want all snippets to be encrypted before being saved into the database, so that I feel confident my code can’t be stolen if the database is compromised
- As a user, I want the snippet to be decrypted before it is returned from the API, so that I can actually read it

### Authentication
- As a user, I want to make an account with my email and password, so that I can have an identity on Snippr.io

### Functional Requirements
#### Encryption
- When a POST request is made to /snippet, the code content of the body should be encrypted before saving in the datastore
- When a GET request is made to /snippet (or any subroute), the code content should be decrypted before returning

### Authentication
- When a POST request is made to /user with email and password in the body, the password should be salted and hashed before the user is saved in the data store.
- ***Bonus***: When a GET request is made to /user, only the user whose correct credentials are provided should be returned. The response must not contain the password (or a hash of the password). This is therefore a protected endpoint and can only be accessed if email and password are provided with the request.

***
## Week 1 MVP:
### User Stories
As a user, I can add a snippet to the data store, so that I can look it up again when I need it.
```
POST/snippet/8 
Returns
{
    "id": 8,
    "language": "C++",
    "code": int main (){
        std::cout << "Hello, Wolrd! << std::endl;
    }
  }
```
As a user, I can request a snippet by its ID, so that I can see the snippet again
```
GET/snippet/8 
Returns
{
    "id": 8,
    "language": "C++",
    "code": int main (){
        std::cout << "Hello, Wolrd! << std::endl;
    }
  }
```
### Functional Requirements:
1. Users can make POST request to /snippet to create a new snippet
2. Users can make a GET request to /snippet to get all the snippets currently in the data store
3. Users can make a GET request to e.g. /snippet/3 to retrieve the snippet with the ID of 3
4. ***Bonus***: Users can make a GET request to e.g. /snippet?lang=python to retrieve all the Python snippets
