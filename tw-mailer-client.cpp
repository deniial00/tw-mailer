#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
 
#include "src/Message.hpp"

int main(int argc, char const* argv[])
{
    const char* IP = argv[1];
    int PORT = std::stoi(argv[2]);
    int sock = 0, valread, client_fd;
    struct sockaddr_in serv_addr;
    char* hello = "Hello from client";
    char buffer[1024] = { 0 };

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
 
    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, IP, &serv_addr.sin_addr) <= 0) {
        printf(
            "\nInvalid address/ Address not supported \n");
        return -1;
    }
 
    if ((client_fd = connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    int option;
    bool running = true;
    // std::string username, reciever, subject, message;
    // std::cout << "established connection!\nEnter username: ";
    // std::cin >> username;
    
    // send
    std::string string = "Daniel Hofbauer\nMario Gussnig\nIst das cool?\nTesty\n.\n";
    // Message request(string, strlen(string));
    
    // std::cout   << "====================" << std::endl 
    //             << "Message to server: " << std::endl 
    //             << "====================" << std::endl
    //             << request.ToString() << std::endl;
                // << "Debug: " << strlen(request.ToString().c_str()) << std::endl;
    
    // int messageLength = strlen(request.ToString().c_str());
    // std::string reqMess = request.ToString();
    // char* mess1 = (char*) reqMess.c_str();
    // printf("Message: %s\n", mess1);

    send(sock, string.c_str(), string.length(), 0);
    
    // read
    memset(buffer, 0x00, strlen(buffer));
    read(sock, buffer, 1024);
    
    std::string response(buffer,strlen(buffer));
    std::cout   << "====================" << std::endl 
                << "Message from server: " << std::endl 
                << "====================" << std::endl
                << response;

    //sends username to server to select correct dir
    // send(sock, username.c_str(), username.length(), 0);

    // while(running){
    //     std::cout << "Select option:\n1. Send Email\n2. Show list of emails\n3. Quit" << std::endl;
    //     std::cin >> option;

    //     switch(option){
    //         case 1:
    //             //receivername must onlyhave 8 letters
    //             do{
    //                 std::cout << "Enter reciever username: ";
    //                 std::cin >> reciever;
    //                 if(reciever.length() > 8){
    //                     std::cout << "Invalid input: Username must not have more than 8 letters!" << std::endl;
    //                 }
    //             }while(reciever.length() > 8);

    //             //receivername must onlyhave 80 letters
    //             do{
    //                 std::cout << "Enter reciever subject: ";
    //                 std::cin >> subject;
    //                 if(reciever.length() > 80){
    //                     std::cout << "Invalid input: Subject must not have more than 80 letters!" << std::endl;
    //                 }
    //             }while(subject.length() > 80);

    //             std::cout << "Enter message:" << std::endl;
    //             std::cin >> message;

    //             //TODO: SEND request to server
    //             send(sock, req->SEND(username, reciever, subject, message), req->SEND(username, reciever, subject, message).length(), 0);
    //             //response = 'OK' or 'ERR'
    //             valread = read(sock, buffer, 1024);

    //             break;
    //         case 2:
    //             while(running){
    //                 std::cout << "List of emails from " << username << std::endl;

    //                 //TODO: LIST request to server
    //                 send(sock, req->LIST(username), req->SEND(username).length(), 0);
    //                 //response count of messages in first line, after that one subject per line
    //                 valread = read(sock, buffer, 1024);

    //                 std::cout << "Select option:\n1. Read Email\n2. Delete Email\n3. Back" << std::endl;
    //                 std::cin >> option;
    //                 switch(option){
    //                     case 1:
    //                         std::cout << "Which message do you want to read? Enter id: ";
    //                         std::cin >> option;

    //                         //TODO: READ request to server
    //                         send(sock, req->READ(username, option), req->SEND(username, option).length(), 0);
    //                         //response 'OK' or 'ERR' in first line; username, reciever, subject, message in the next 4 lines
    //                         valread = read(sock, buffer, 1024);

    //                         break;
    //                     case 2:
    //                         std::cout << "Which message do you want to delete? Enter id: ";
    //                         std::cin >> option;

    //                         //TODO: DELETE request to server
    //                         send(sock, req->READ(username, option), req->SEND(username, option).length(), 0);
    //                         //response 'OK' or 'ERR'
    //                         valread = read(sock, buffer, 1024);

    //                         break;
    //                     case 3:
    //                         running = false;
    //                         break;
    //                     default:
    //                         std::cout << "Invalid input!" << std::endl;
    //                 }
                    
    //             }
    //             running = true;
    //             break;
    //         case 3:
    //             running = false;
    //             close(client_fd);
    //             std::cout << "Connection closed" << std::endl;
    //             break;
    //         default:
    //             std::cout << "Invalid input!" << std::endl;
    //     }
    // }
    // send(sock, hello, strlen(hello), 0);
    // printf("Hello message sent\n");
    // valread = read(sock, buffer, 1024);
    // printf("%s\n", buffer);
 
    // closing the connected socket
    close(client_fd);
    return 0;
}