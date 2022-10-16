#include "ServerController.hpp"

ServerController::ServerController(int port)
{
    std::cout << "Creating server ...";

    int opt = 1;

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

ServerController::ServerController(int port, std::string baseDir) 
: ServerController::ServerController(port)
{
    _baseDir = baseDir;
}

void ServerController::Listen()
{
    
    int socket;
    // char buffer[1024];
    int addrSize = sizeof(this->_address);

    this->_isRunning = true;

    while(this->_isRunning)
    {
        std::cout << "listening..." << std::endl;

        if (listen(this->_server, 3) < 0) {
            perror("error while listening");
            exit(EXIT_FAILURE);
        }

        if ((socket = accept(this->_server, (struct sockaddr*)&this->_address, (socklen_t*)&addrSize)) < 0) {
            perror("error while accepting");
            exit(EXIT_FAILURE);
        }

        // read from socket
        char buffer[1024] = {0};
        int received = 0;
        // TODO: umbau auf max sec?
        received = read(socket, buffer, strlen(buffer));
        printf("Message: %s\n", buffer);
        Message request(buffer, received);
        std::cout   << "====================" << std::endl 
                    << "Message from client: " << std::endl 
                    << "====================" << std::endl
                    << request.ToString();

        // write to socket
        const char* temp = "Mario Gussnig\nDaniel Hofbauer\nVoll!\nTESTYYY\n.\n";
        Message message(temp, strlen(temp));

        std::cout   << "====================" << std::endl 
                    << "Response to client: " << std::endl 
                    << "====================" << std::endl
                    << message.ToString();

        this->SendMessage(socket, message);
        
        close(socket);
    }
}
 
int ServerController::SendMessage(int socket, Message message)
{
    int messageLength = strlen(message.ToString().c_str());
    std::string mess = message.ToString();
    char* mess1 = (char*) mess.c_str();

    // return -1 if send failed
    if(send(socket, mess1, messageLength, 0) == -1)
        return -1;

    return 1;
}

Message ServerController::ReceiveMessage(int socket)
{
    char buffer[1024] = {0};
    int received = 0;
    // TODO: umbau auf max sec?
    received = read(socket, buffer, strlen(buffer));
    printf("Message: %s\n", buffer);
    Message response(buffer, received);
    
    return response;
}

std::vector<Message> ServerController::GetMessages(std::string name)
{
    std::vector<Message> messagesSent = GetMessagesFromDir(name);
    std::vector<Message> messagesReceived;

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

    std::string path = this->_baseDir + name;

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
        

        // Message message("Daniel Hofbauer\nMario Gussnig\nIst das cool?\nTesty\n.\n");

        // messages.push_back(message);

    }

    closedir(dir_ptr);

    return messages;
}