#include<iostream>

class Request{
    public:
        	Request();

            std::string SEND(std::string username, std::string reciever, std::string subject, std::string message){
                return "SEND\n" + username + "\n" + reciever + "\n" + subject + "\n" + message + "\n.\n";
            };
            std::string LIST(std::string username){
                return "LIST\n" + username + "\n";
            };
            std::string READ(std::string username, int option){
                return "READ\n" + username + "\n" + std::to_string(option) + "\n";
            };
            std::string DEL(std::string username, int option){
                return "READ\n" + username + "\n" + std::to_string(option) + "\n";
            };
};