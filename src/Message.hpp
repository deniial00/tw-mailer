#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <string.h>

class Message
{
    private:
    std::string _identifier;
    std::string _sender;
    std::string _receiver;
    std::string _subject;
    std::string _message;
    void ParseData(std::string data);
    
    public:
    // Message(std::string id);
    Message(const char* data, int length);
    void SetSender(std::string sender);
    void SetReceiver(std::string sender);
    int SetSubject(std::string sender);
    void SetMessage(std::string sender);
    std::string ToString();
};

