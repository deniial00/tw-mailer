#include "ServerController.hpp"

ServerController::ServerController(int port)
{
    std::cout << "Creating server ...";

    int opt = 1;

    // set valid address
    this->address.sin_family = AF_INET;
    this->address.sin_addr.s_addr = INADDR_ANY;
    this->address.sin_port = htons(port);

    // Creating socket file descriptor
    if ((this->serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(this->serverSocket, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (bind(this->serverSocket, (struct sockaddr*)&this->address, sizeof(this->address)) < 0) {
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
    int client;
    int addrSize = sizeof(this->address);

    this->isRunning = true;

    std::cout << "listening..." << std::endl;
    
    // loop while running
    while(this->isRunning)
    {
        // create buffer
        char buffer[1024] = {0};

        if (listen(this->serverSocket, 3) < 0) {
            std::cout << "[Error] could not listen on port " << this->address.sin_port << std::endl;
            exit(EXIT_FAILURE);
        }

        if ((client = accept(this->serverSocket, (struct sockaddr*)&this->address, (socklen_t*)&addrSize)) < 0) {
            std::cout << "[Error] Could not accept client" << std::endl;
            close(client);
            continue;
        }

        // read from socket
        read(client, buffer, 1024);
        std::string request(buffer,strlen(buffer));

        std::cout   << "==================" << std::endl 
                    << std::endl << "Request from client:" << std::endl
                    << request << std::endl << std::endl;

        // handle request and return responseString
        std::string response = this->HandleRequest(client, request);

        std::cout   << "Response to client:" << std::endl
                    << response << std::endl << std::endl;

        // write to socket
        send(client, response.c_str(), response.length(),0);
        
        close(client);
    }
}
 
std::string ServerController::HandleRequest(int client, std::string requestString)
{

    std::string header, body, response = "";

    try {
        header = requestString.substr(0, requestString.find('\n'));
        body = requestString.substr(requestString.find("\n") + 1, std::string::npos);
    } catch (...) {
        std::cout << "[ERROR] Could not parse message";
        return "ERR\n";
    }
    
    if(header == "SEND") {
        // create and rint message
        Message message(body);
        std::cout << "body: " << message;
        
        // print messageage
        std::cout << message;

        // store to inbox of receiver and outbox of sender
        response = this->StoreMessageToDir(message,message.GetReceiver(), "inbox");
        response = this->StoreMessageToDir(message,message.GetSender(), "outbox");


    } else if (header == "LIST") {
        // retrieve outbox and inbox of user
        std::vector<Message> list = this->GetMessages(body);

        // check if messages were found
        if(list.size() == 0) {
            response = "ERR\n";
            return response;
        }

        // amount of messages
        response = "Results: " + std::to_string(list.size()) + "\n";
        
        // all message subjects
        for (auto result = list.begin(); result < list.end(); result++){
            int index = std::distance(list.begin(), result);
            response.append(std::to_string(index) + ": " + result->GetSubject() + "\n");
        }
        
    } else if (header == "READ") {
        // split string on first newline
        std::string username = body.substr(0, body.find('\n'));
        
        // get next line and convert to int
        int index = std::stoi(body.substr(body.find("\n") + 1, std::string::npos));
        
        // get messages from user (inbox and outbox)
        std::vector<Message> list = this->GetMessages(username);

        // check if empty if not then return message
        if(index > list.size() || list.size() == 0){
            response = "ERR\n";
        } else {
            // instantiate message
            Message temp = list.at(index);

            // return ok and message
            response = "OK\n"; 
            response += temp.ToString();
        }

    } else if (header == "DEL") {
        // split body at new line
        std::string username = body.substr(0, body.find('\n'));

        // get next line and convert to int
        int index = std::stoi(body.substr(body.find("\n") + 1, std::string::npos));

        // get messages from user (inbox and outbox)
        std::vector<Message> list = this->GetMessages(username);
        
        // instantiate message
        Message messageToDelete = list.at(index);

        // delete message and return corresponding code
        if( this->DeleteMessage(messageToDelete) == -1 ) {
            response = "ERR\n";
        } else {
            response = "OK\n";
        }

    } else {
        // Should never happen.
        std::cout << "[ERROR] Wrong Request Type" << std::endl;
        response = "ERR\n";
    }

    return response;

}

int ServerController::StoreMessageToDir(Message message, std::string user, std::string subfolder)
{
    FILE* file;
    errno = 0;

    std::string filePath = this->baseDir + "/" + user;
    mkdir(filePath.c_str(), 0777);

    filePath += "/" + subfolder;
    mkdir(filePath.c_str(), 0777);

    filePath += "/" + message.GetIdentifier();

    if((file = fopen(filePath.c_str(), "w")) == nullptr) {
        printf("Error: %d (%s)\n", errno, strerror(errno));
        std::cout << "[ERROR] Could not open file \"" << filePath << "\"" << std::endl;
        return -1;
    }

    if(fprintf(file, "%s", message.ToString().c_str()) < 0 ) {
        std::cout << "[ERROR] Could not write to file \"" << filePath << "\"" << std::endl;
        return -1;
    }

    fclose(file);

    return 1;
}

std::vector<Message> ServerController::GetMessages(std::string name)
{
    std::vector<Message> messagesCombined;
   
    // remove newlines from name
    name.erase(std::remove(name.begin(), name.end(), '\n'), name.cend());

    // get inbox and outbox of user
    std::vector<Message> messagesSent = this->GetMessagesFromDir(name + "/inbox");
    std::vector<Message> messagesReceived = this->GetMessagesFromDir(name + "/outbox");

    // merge both vectors
    messagesCombined.reserve(messagesReceived.size() + messagesSent.size());
    messagesCombined.insert(messagesCombined.end(), messagesSent.begin(), messagesSent.end());
    messagesCombined.insert(messagesCombined.end(), messagesReceived.begin(), messagesReceived.end());

    // print error if no messages found 
    if(messagesCombined.size() == 0) {
        std::cout << "[ERROR] No Messages found" << std::endl;
    }

    return messagesCombined;
}

int ServerController::DeleteMessage(Message message)
{
    int result = 0;

    // create absolute filepath and remove
    std::string filePathOutbox = this->baseDir + "/" + message.GetSender() + "/outbox/" + message.GetIdentifier();
    result += remove(filePathOutbox.c_str());

    // create absolute filepath and remove
    std::string filePathInbox = this->baseDir + "/" + message.GetReceiver() + "/inbox/" + message.GetIdentifier();
    result += remove(filePathInbox.c_str());

    // both functions should return 0 on success
    if(result != 0)
        return -1;
    
    return 0;
}

std::vector<Message> ServerController::GetMessagesFromDir(std::string filepath)
{
    std::vector<Message> messages;

    struct dirent* dirent_ptr;
    DIR* dir_ptr;
    FILE* file;

    // create dirpath
    filepath = this->baseDir + "/" + filepath;

    // open dir or return if unsuccessful
    if ((dir_ptr = opendir(filepath.c_str())) == NULL){
        std::cout << "[ERROR] Could not open directory: \"" << filepath << "\"" << std::endl;
        return messages;
    }

    // read dir and loop over all elements inside folder
    while ((dirent_ptr = readdir(dir_ptr)) != NULL) {
        char buffer[2048];
        std::string fileName = dirent_ptr->d_name;

        // these two elements can be skipped
        if(fileName.compare(".") == 0 || fileName.compare("..") == 0)
            continue;
        
        // create filepath
        std::string absoluteFileName = filepath + "/" + fileName;
        
        // open file
        if((file = fopen(absoluteFileName.c_str(), "r")) == nullptr) {
            printf("Error: %d (%s)\n", errno, strerror(errno));
            std::cout << "[ERROR] Could not open file \"" << absoluteFileName << "\"" << std::endl;
            continue;
        }

        // append lines to messageString
        std::string messageString = "";
        while( (fgets(buffer , 2048 , file)) != NULL) {
            messageString.append(buffer);
        }

        // instantiate message and add to vector
        Message temp(messageString, fileName);
        messages.push_back(temp);

        // close file
        fclose(file);
    }

    // close dir 
    closedir(dir_ptr);

    // return message
    return messages;
}