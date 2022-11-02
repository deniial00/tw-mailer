#include "src/ClientController.hpp"

int main(int argc, char const* argv[])
{
    if(argc != 3) {
        std::cout << "Port or IP not provided" << std::endl;
        exit(EXIT_FAILURE);
    }
    // set params of client
    const char* ip = (const char*) argv[1];
    int port = std::stoi(argv[2]);
    ClientController* client = new ClientController(port, ip);
    
    std::string input;

    // loop as long as input is not Quit or Q
    while (input != "Quit" || toupper(input.at(0)) != 'Q')
    {
        // menu 
        input = "";
        std::cout << "Choose action: SEND, LIST, READ, DEL, QUIT: ";
        std::getline(std::cin, input);
        
        // loop until input matches Commands
        while(input != "SEND" && input != "LIST" && input != "DEL" && input != "READ" && input != "QUIT") {
            // print error if invalid
            std::cout << "Invalid Input. Try again: ";

            std::getline(std::cin, input);
        }
        
        // create request
        std::string requestString = client->CreateRequest(input);

        // send previously created request to server and return the servers response
        std::string response = client->SendString(requestString);
        
        // print server response
        std::cout << response << std::endl;
    }
}