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

    std::string response = "";

    switch(mode.at(0))
    {
        // 83 == S
        case 83:
        {
            // send message
            std::string sender, receiver, subject, message = "";
            std::string messageLine;

            std::cout << "Sender:   ";
            std::cin >> sender;
            std::cout << "Receiver: ";
            std::cin >> receiver;
            std::cout << "Subject:  ";
            std::cin >> subject;
            std::cout << "Message: (end with '.' + ENTER)" << std::endl;

            while (std::getline(std::cin, messageLine))
            {
                if (messageLine == ".") {
                    break;
                }
                message.append(messageLine + "\n");
            }

            std::cout << "Message:" << std::endl << message;
            response = Request::SEND(sender, receiver, subject, message);
            break;
        }
        // 76 == L
        case 76:
            // list messages
            break;
        // 82 == R
        case 82:
            // read message
            break;
        // 68 == D
        case 68:
            // delete message
            break;
        // 81 == Q
        case 81:
            // quit
            std::cout << "Shutting down ... Ok" << std::endl;
            exit(EXIT_SUCCESS);
            break;
        // invalid
        default:
            std::cout << std::endl << "Invalid Input. Try again" << std::endl;
        break;
    }
        
    return response;
}