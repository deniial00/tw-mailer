#include "ClientController.hpp"

ClientController::ClientController() { }

ClientController::ClientController(int port, const char* ip)
{
    std::cout << "Starting client ...";

    // Convert IPv4 and IPv6 addresses from text to binary
    if (inet_pton(AF_INET, ip, &(this->serverAddress.sin_addr)) <= 0) {
        perror("Invalid address/Address not supported");
        exit(EXIT_FAILURE);
    }

    this->serverAddress.sin_family = AF_INET;
    this->serverAddress.sin_port = htons(port);

    std::cout << "ok" << std::endl;

    // this->SendRequest("SEND\nnew sock");
}

std::string ClientController::SendRequest(std::string message)
{
    char buffer[BUFFER_SIZE] = { 0 };
    int socketDescriptor;
    
    // create serverSocket
    if ((this->serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror(" Socket creation error");
        exit(EXIT_FAILURE);
    }
    
    // connect to server socket
    if ((socketDescriptor = connect(this->serverSocket, (struct sockaddr*)& (this->serverAddress), sizeof(this->serverAddress))) < 0) {
        perror("Connection Failed");
        exit(EXIT_FAILURE);
    }

    // send
    send(this->serverSocket, message.c_str(), strlen(message.c_str()), 0);

    // read
    read(this->serverSocket, buffer, 1024);
    std::string response(buffer, strlen(buffer));

    // close connection
    close(this->serverSocket);
    this->serverSocket = -1;

    return response;
}

std::string ClientController::CreateRequest(std::string mode) 
{
    // if(mode == NULL)
    //     return -1;

    std::string response, username = "", option;

    if(mode == "SEND") {
        // send message
        std::string sender, receiver, subject, message = "";
        std::string messageLine;

        std::cout << "Sender:   ";
        // std::cin >> sender;
        std::getline(std::cin, sender);

        std::cout << "Receiver: ";
        // std::cin >> receiver;
        std::getline(std::cin, receiver);
        
        do {
            std::cout << "Subject:  ";
            std::getline(std::cin, subject);
        } while(subject.length() > 80);

        std::cout << "Message: (end with '.' + ENTER)" << std::endl;
        
        // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        while (std::getline(std::cin, messageLine))
        {
            if (messageLine == ".") {
                break;
            }
            message.append(messageLine + "\n");
        }

        response = Request::SEND(sender, receiver, subject, message);
    } else if(mode == "LIST") {
        //get list of messages
        std::cout << "Username:   ";
        std::getline(std::cin, username);
        response = Request::LIST(username);

    } else if(mode == "READ") {
        //read message at index
        std::cout << "Username:   ";
        std::getline(std::cin, username);

        std::cout << "Enter index of email you want to read:   ";
        std::getline(std::cin, option);
        
        response = Request::READ(username, option);

    } else if(mode == "DEL") {
        //delete message at index
        std::cout << "Username:   ";
        std::getline(std::cin, username);
        
        std::cout << "Enter index of email you want to delete:   ";
        std::getline(std::cin, option);
        
        response = Request::DEL(username, option);

    } else if(mode == "QUIT") {
        std::cout << "Shutting down ... Ok" << std::endl;
        exit(EXIT_SUCCESS);
    } else {
        std::cout << std::endl << "Invalid Input. Try again" << std::endl;
    }

    return response;
}