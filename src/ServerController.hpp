#pragma once

// io
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>

// general
#include <stdlib.h>
#include <errno.h>
// #include <string.h>
#include <vector>

// dir
#include <dirent.h>
#include <sys/stat.h>

#include "Message.hpp"

class ServerController
{
    private:
    std::string baseDir;
    bool isRunning;
    int serverSocket;
    struct sockaddr_in address;

    std::string HandleRequest(int client);
    std::vector<Message> GetMessagesFromDir(std::string filepath);
    int StoreMessageToDir(Message message, std::string user, std::string subfolder);

    public:
    ServerController(int port);
    ServerController(int port, std::string baseDir);
    void Listen();
    std::vector<Message> GetMessages(std::string name);
    int DeleteMessage(Message message);
};