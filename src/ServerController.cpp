#include "ServerController.hpp"

ServerController::ServerController(int port)
{
    std::cout << "Creating server ...";

    int new_socket, valread, opt = 1;

    this->_address.sin_family = AF_INET;
    this->_address.sin_addr.s_addr = INADDR_ANY;
    this->_address.sin_port = htons(port);

    // Creating socket file descriptor
    if ((this->_server = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(this->_server, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (bind(this->_server, (struct sockaddr*)&this->_address, sizeof(this->_address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "ok" << std::endl;
}

void ServerController::Listen()
{
    
    int socket;
    char buffer[1024];
    int addrSize = sizeof(this->_address);

    this->_isRunning = true;

    while(this->_isRunning)
    {
        std::cout << "listening..." << std::endl;
        
        if (listen(this->_server, 3) < 0) {
            perror("listen");
            exit(EXIT_FAILURE);
        }

        if ((socket = accept(this->_server, (struct sockaddr*)&this->_address, (socklen_t*)&addrSize)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        read(socket, buffer, 1024);

        std::cout << buffer << std::endl;

        std::string message = "ACK";
        send(socket, message.c_str(), strlen(message.c_str()),0);

        close(socket);
    }
}