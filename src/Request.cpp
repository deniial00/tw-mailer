#include "Request.h"

Request::Request() { }

Request::Request(std::string data)
{
    // Parse data into object
}

Request::Request(MessageType messageType)
{
    _messageType = messageType;
}

Request::Request(MessageType messageType, std::string data) : Request::Request(data)
{
    _messageType = messageType;
}

void Request::AddMessageType(MessageType messageType)
{
    try {
        _messageType = messageType;
    }
    catch (...){
        throw "Incorrect messageTyp"; 
    }
}

void Request::AddLine(std::string line)
{
    _lines.push_back(line);
}

