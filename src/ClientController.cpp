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

    // this->SendRequest("SEND\nnew sock");
}

bool ClientController::getLoggedIn()
{
    return this->loggedIn;
}

void ClientController::setLoggedIn(bool option)
{
    this->loggedIn = option;
}

std::string ClientController::SendRequest(std::string message)
{
    char buffer[BUFFER_SIZE] = { 0 };
    int socketDescriptor;
    
    // create serverSocket
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
    send(this->serverSocket, message.c_str(), message.length(), 0);

    // read
    read(this->serverSocket, buffer, 1024);
    std::string response(buffer, strlen(buffer));

    // close connection
    close(this->serverSocket);
    this->serverSocket = -1;

    return response;
}

std::string ClientController::CreateRequest(std::string mode) 
{
    // if(mode == NULL)
    //     return -1;

    std::string response, option;

    // check if user is already logged in
    if(mode != "QUIT" && mode != "LOGIN" && loggedIn == false){
        std::cout << "You must log in to proceed\n";
        return "error";
    } else if(mode == "LOGIN"){
        std::string ldap_username, ldap_password;

        std::cout << "Username: ";
        std::cin >> ldap_username;
        this->username = ldap_username;

        std::cout << "Password: ";
        std::cin >> ldap_password;

        response = Request::LOGIN(ldap_username, ldap_password);   
    } else if(mode == "SEND") {
        // send message
        std::string receiver, subject, message = "";
        std::string messageLine;


        std::cout << "Receiver: ";
        std::cin >> receiver;
        
        do {
            std::cout << "Subject:  ";
            std::cin >> subject;
        } while(subject.length() > 80);

        std::cout << "Message: (end with '.' + ENTER)" << std::endl;
        
        // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        while (std::getline(std::cin, messageLine))
        {
            if (messageLine == ".") {
                break;
            }
            message.append(messageLine + "\n");
        }

        response = Request::SEND(this->username, receiver, subject, message);
    } else if(mode == "LIST") {
        // list all inbound and outbound messages
        response = Request::LIST(this->username);

    } else if(mode == "READ") {
        //read message at index

        std::cout << "Enter index of the email you want to read:   ";
        std::cin >> option;
        
        response = Request::READ(username, option);

    } else if(mode == "DEL") {
        //delete message at index
        
        std::cout << "Enter index of email you want to delete:   ";
        std::cin >> option;
        
        response = Request::DEL(this->username, option);

    } else if(mode == "QUIT") {
        std::cout << "Shutting down ... Ok" << std::endl;
        exit(EXIT_SUCCESS);
    } else {
        std::cout << std::endl << "Invalid Input. Try again" << std::endl;
        response = "error";
    }

    return response;
}