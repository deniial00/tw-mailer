#include <stdio.h>
#include <string>
#include <vector>
#include <exception>
#include "Message.h"
#include "MessageType.h"

class Request : public Message
{
    MessageType _messageType;

    std::vector<std::string> _lines;

    Request();

    Request(std::string data);

    Request(MessageType messageType);

    Request(MessageType messageType, std::string data);

    void AddMessageType(MessageType messageType);

    void AddLine(std::string line);
};