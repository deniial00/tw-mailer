#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <string>
 
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

    //send request
    int option;
    bool running = true;
    std::string username, reciever, message;
    std::cout << "established connection!\nEnter username: ";
    std::cin >> username;
    //sends username to server to select correct dir
    send(sock, username.c_str(), username.length(), 0);

    while(running){
        std::cout << "Select option:\n1. Send Email\n2. Show list of emails\n3. Quit" << std::endl;
        std::cin >> option;

        switch(option){
            case 1:
                //TODO: max 8 chars
                do{
                    std::cout << "Enter reciever username: ";
                    std::cin >> reciever;
                    if(reciever.length() > 8){
                        std::cout << "Invalid input: Username must not have more than 8 letters!" << std::endl;
                }
                }while(reciever.length() > 8);
                std::cout << "Enter message:" << std::endl;
                std::cin >> message;
                //TODO: SEND request to server
                break;
            case 2:
                while(running){
                    std::cout << "List of emails from " << username << std::endl;
                    //TODO: LIST request to server

                    std::cout << "Select option:\n1. Read Email\n2. Delete Email\n3. Back" << std::endl;
                    std::cin >> option;
                    switch(option){
                        case 1:
                            std::cout << "Which message do you want to read? Enter id: ";
                            std::cin >> option;
                            //TODO: READ request to server
                            break;
                        case 2:
                            std::cout << "Which message do you want to delete? Enter id: ";
                            std::cin >> option;
                            //TODO: DELETE request to server
                            break;
                        case 3:
                            running = false;
                            break;
                        default:
                            std::cout << "Invalid input!" << std::endl;
                    }
                    
                }
                running = true;
                break;
            case 3:
                running = false;
                close(client_fd);
                std::cout << "Connection closed" << std::endl;
                break;
            default:
                std::cout << "Invalid input!" << std::endl;
        }
    }
    // send(sock, hello, strlen(hello), 0);
    // printf("Hello message sent\n");
    // valread = read(sock, buffer, 1024);
    // printf("%s\n", buffer);
 
    // // closing the connected socket
    // close(client_fd);
    return 0;
}