#include <netinet/in.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>

class ServerController
{
    private:
    bool _isRunning;
    int _server;
    std::string _baseDir;
    struct sockaddr_in _address;

    void HandleRequest(int socket);
    void Send(int socket, std::string message);

    public:
    ServerController(int port);
    void Listen();

};