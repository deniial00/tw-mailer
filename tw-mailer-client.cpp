#include "src/ClientController.hpp"

int main(int argc, char const* argv[])
{
    if(argc != 3) {
        std::cout << "Port or IP not provided" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    const char* ip = (const char*) argv[1];
    int port = std::stoi(argv[2]);
    ClientController* client = new ClientController(port, ip);
    
    std::string input;
    std::string response;

    while (input != "Quit" || toupper(input.at(0)) != 'Q')
    {
        do {
            if( client->getLoggedIn() == false){
            std::cout << "Choose action: LOGIN, SEND, LIST, READ, DEL, QUIT: ";
            }
            else{
                std::cout << "Choose action: SEND, LIST, READ, DEL, QUIT: ";
            }
            //std::getline(std::cin, input);
            std::cin >> input;

            if(input != "LOGIN" && input != "SEND" && input != "LIST" && input != "DEL" && input != "READ" && input != "QUIT"){
                std::cout << "Invalid Input.\n";
            }
        } while(input == "");
        
        std::string request = client->CreateRequest(input);
        if(request != "error"){
            response = client->SendRequest(request);
        }

        if(input == "LOGIN" && response == "OK\n"){
            client->setLoggedIn(true);
        }

        std::cout << std::endl << response << std::endl;
        sleep(2);
    }
}