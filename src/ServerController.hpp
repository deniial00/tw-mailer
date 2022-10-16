#pragma once

// io
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>

// general
#include <stdlib.h>
// #include <string.h>
#include <vector>

// dir
#include <dirent.h>
#include <sys/stat.h>
#include <fnmatch.h>

#include "Message.hpp"

class ServerController
{
    private:
    std::string _baseDir;
    bool _isRunning;
    int _server;
    struct sockaddr_in _address;

    void HandleRequest(int socket);
    int SendMessage(int socket, Message message);
    Message ReceiveMessage(int socket);
    std::vector<Message> GetMessagesFromDir(std::string name);

    public:
    ServerController(int port);
    ServerController(int port, std::string baseDir);
    void Listen();
    std::vector<Message> GetMessages(std::string name);
    int StoreMessage(Message message);
    int DeleteMessage(Message message);

};