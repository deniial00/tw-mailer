#include "ServerController.hpp"

ServerController::ServerController(int port)
{
    std::cout << "Creating server ...";

    int opt = 1;

    this->address.sin_family = AF_INET;
    this->address.sin_addr.s_addr = INADDR_ANY;
    this->address.sin_port = htons(port);

    // Creating socket file descriptor
    if ((this->server = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(this->server, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (bind(this->server, (struct sockaddr*)&this->address, sizeof(this->address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "ok" << std::endl;
}

ServerController::ServerController(int port, std::string baseDir) 
: ServerController::ServerController(port)
{
    this->baseDir = baseDir;
}

void ServerController::Listen()
{
    int client, receivedBytes = 0;
    int addrSize = sizeof(this->address);

    this->isRunning = true;

    while(this->isRunning)
    {
        std::cout << "listening..." << std::endl;

        if (listen(this->server, 3) < 0) {
            perror("error while listening");
            exit(EXIT_FAILURE);
        }

        if ((client = accept(this->server, (struct sockaddr*)&this->address, (socklen_t*)&addrSize)) < 0) {
            perror("error while accepting");
            exit(EXIT_FAILURE);
        }

        // read from socket
        int ret = this->HandleRequest(client);

        // write to socket
        std::string response;

        switch(ret) 
        {
            case -1:
                response = "Error\n";
            case 1:
                response = "OK\n";
                break;
        }
        
        printf("====================\nMessage to client:\n%s \n====================\n", response.c_str());
        send(client, response.c_str(), response.length(),0);
        
        close(client);
    }
}
 
// int ServerController::SendMessage(int socket, Message message)
// {
//     int messageLength = strlen(message.ToString().c_str());
//     std::string mess = message.ToString();
//     char* mess1 = (char*) mess.c_str();

//     // return -1 if send failed
//     if(send(socket, mess1, messageLength, 0) == -1)
//         return -1;

//     return 1;
// }

int ServerController::HandleRequest(int client)
{
    char buffer[1024] = {0};
    int receivedBytes;

    // TODO: umbau auf max sec?
    receivedBytes = read(client, buffer, 1024);
    std::string request(buffer,strlen(buffer));
    std::cout   << "====================" << std::endl 
                << "Message from client:" << std::endl
                << request << std::endl
                << "====================" << std::endl;

    std::string header = request.substr(0, request.find("\n"));
    std::string body = request.substr(request.find("\n"), 0);

    // TODO: ADD functions to handle request types
    // if(header == "SEND") {

    // } else if (header == "LIST") {

    // } else if (header == "READ") {

    // } else if (header == "DEL") {

    // } else {
    //     std::cout << "[ERROR] Wrong Request Type" << std::endl;
    // }

    return 1;
}

std::vector<Message> ServerController::GetMessages(std::string name)
{
    std::vector<Message> messagesSent = GetMessagesFromDir(name + "/outgoing/");
    std::vector<Message> messagesReceived = GetMessagesFromDir(name + "/ingoing/");

    std::vector<Message> messagesCombined;

    messagesCombined.reserve(messagesReceived.size() + messagesSent.size());
    messagesCombined.insert(messagesCombined.end(), messagesSent.begin(), messagesSent.end());
    messagesCombined.insert(messagesCombined.end(), messagesReceived.begin(), messagesReceived.end());

    return messagesCombined;
}

int ServerController::StoreMessage(Message message)
{
    return 1;
}

// int ServerController::DeleteMessage(Message message)
// {
//     fnmatch();
//     return 1;
// }

std::vector<Message> ServerController::GetMessagesFromDir(std::string name)
{
    std::vector<Message> messages;

    struct dirent* dirent_ptr;
    // struct stat statbuf;
    DIR* dir_ptr;

    std::string path = this->baseDir + name;

    // open dir or return if unsuccessful
    if ((dir_ptr = opendir(path.c_str())) == NULL){
        return messages;
    }

    // read dir and loop over all elements inside folder
    while ((dirent_ptr = readdir(dir_ptr)) != NULL) {
        std::string fileName = dirent_ptr->d_name;

        // these two elements can be skipped
        if(fileName.compare(".") == 0 || fileName.compare("..") == 0)
            continue;
        
        // TODO: open file and store in vector

    }

    closedir(dir_ptr);

    return messages;
}