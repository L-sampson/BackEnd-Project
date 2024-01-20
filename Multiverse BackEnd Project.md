# Multiverse BackEnd Project
## TechStack
1. **C++ STL fstream** 
    - Open Seed Data Files.
2. **Curl Library** 
    - Client-Side API/HTTP Request(GET, POST, PUT, DELETE)
3. **Crow Library**
    - Server-Side API/HTTP
4. **Modern C++ Json(Nlohmann)**
    - Parse JSON Objects
5. **GTest**
    - Testing Framework
6. **Protobuf(Potential Stack)**
    - Create Messages to Serialize & Deserialize Data
7. **OpenSSL**
    - Encryption for password and username
8. **Botan**
    - Encryption

### FileTree Structure
``` bash
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
    |   |   |-- Server.cpp           // Server implementation using Crow
    |   |-- utils/
    |   |   |-- AuthenticateUser.cpp // Creates Users, hashes password and Authenticate their login.
    |   |   |-- Encrypt.cpp          // Encrypt & Decrypt data sent using HTTP requests
    |   |   |-- EncryptionKey.cpp    // Provides env keys for encryption/decryption
    |   |   |-- JSONParser.cpp       // JSON parsing logic for server-side, reading files.
    |   |   |-- JSONParser.h         // Header file for the JSONParser class used by the server
    |   |-- tests/                   // Server-side unit tests (if applicable)
    |       |-- test_routes.cpp      // Unit test file for server-side route handling (if applicable)
    |
    |-- data/
    |   |-- seedData.json                // Seed data file for code snipepts
    |   |-- seedUsers.json               // Seed data file for users

    |-- README.md                    // Project documentation
    |-- Bazel/CMake BuildFile        // Build automation script
```

## Week 3 MVP: ***Find a safe way of allowing secured access without providing their password every time***

### User Stories
- As a user, I want to authenticate once with email and password, so that I can access my snippets without writing my password every time
### Functional Requirements Week 3
- The user provides their email and password to authenticate and receives a token in exchange
- The token allows the user to authorize further requests to the API without repeating their password
- Users can access secured resources with their token just as they could using their password in the previous epic
- The token expires after 24 hours
- Email and password should be passed in the Authorization header of the request
- Use JWTs to provide authorization

## Week 2 MVP
### User Stories Week 2
#### Encryption
- As a user, I want all snippets to be encrypted before being saved into the database, so that I feel confident my code can’t be stolen if the database is compromised
- As a user, I want the snippet to be decrypted before it is returned from the API, so that I can actually read it

### Authentication
- As a user, I want to make an account with my email and password, so that I can have an identity on Snippr.io

### Functional Requirements Week 2
#### Encryption MVP
- When a POST request is made to /snippet, the code content of the body should be encrypted before saving in the datastore
- When a GET request is made to /snippet (or any subroute), the code content should be decrypted before returning

### Authentication MVP
- When a POST request is made to /user with email and password in the body, the password should be salted and hashed before the user is saved in the data store.
- ***Bonus***: When a GET request is made to /user, only the user whose correct credentials are provided should be returned. The response must not contain the password (or a hash of the password). This is therefore a protected endpoint and can only be accessed if email and password are provided with the request.

***
## Week 1 MVP
### User Stories Week 1
As a user, I can add a snippet to the data store, so that I can look it up again when I need it.
```bash
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
```bash
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
### Functional Requirements Week 1
1. Users can make POST request to /snippet to create a new snippet
2. Users can make a GET request to /snippet to get all the snippets currently in the data store
3. Users can make a GET request to e.g. /snippet/3 to retrieve the snippet with the ID of 3
4. ***Bonus***: Users can make a GET request to e.g. /snippet?lang=python to retrieve all the Python snippets
