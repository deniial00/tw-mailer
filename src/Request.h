#include <string>
#include <vector>
#include "Message.h"
#include "MessageType.h"

class Request : public Message
{
    MessageType _messageType;

    std::vector<std::string> _lines;

    Request();

    Request(std::string data);

    Request(MessageType statusCode);

    bool AddMessageType(MessageType messageType);

    bool AddLine(std::string line);
};