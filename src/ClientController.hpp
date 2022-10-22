#pragma once

// io
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

#include <string>


#define BUFFER_SIZE 1024

class ClientController
{
    private:
    struct sockaddr_in serverAddress;
    int serverSocket;

    public:
    std::string SendRequest(std::string message);
    ClientController();
    ClientController(int port, const char* ip);
};