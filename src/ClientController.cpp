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

    this->SendRequest("SEND\nnew sock");
}

std::string ClientController::SendRequest(std::string message)
{
    char buffer[BUFFER_SIZE] = { 0 };
    int socketDescriptor;
    
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

    close(this->serverSocket);
    this->serverSocket = -1;

    return response;
}