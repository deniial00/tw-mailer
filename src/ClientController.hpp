#pragma once

// io
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

#include <string>

#include "Request.hpp"


#define BUFFER_SIZE 1024

class ClientController
{
    private:
    struct sockaddr_in serverAddress;
    int serverSocket;

    public:
    ClientController();
    ClientController(int port, const char* ip);
    std::string CreateRequest(std::string mode);
    std::string SendString(std::string message);
};