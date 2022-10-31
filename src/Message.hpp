#pragma once

#include <string>
#include <chrono>
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
    Message(std::string data);
    void SetSender(std::string sender);
    void SetReceiver(std::string receiver);
    int SetSubject(std::string subject);
    void SetMessage(std::string message);
    std::string SetIdentifier(std::string identifier = "");
    std::string GetSender();
    std::string GetReceiver();
    std::string GetSubject();
    std::string GetIdentifier();
    std::string ToString();
};

