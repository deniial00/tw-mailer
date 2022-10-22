#include <iostream>

class Request{

    public:
    std::string static SEND(std::string username, std::string reciever, std::string subject, std::string message)
    {
        return "SEND\n" + username + "\n" + reciever + "\n" + subject + "\n" + message + ".\n";
    };

    std::string static LIST(std::string username)
    {
        return "LIST\n" + username + "\n";
    };

    std::string static READ(std::string username, int option)
    {
        return "READ\n" + username + "\n" + std::to_string(option) + "\n";
    };

    std::string static DEL(std::string username, int option)
    {
        return "READ\n" + username + "\n" + std::to_string(option) + "\n";
    };

};
