#include <string>
#include <vector>
#include "Message.h"
#include "ResponseCode.h"

class Response : public Message
{
    ResponseCode _statusCode;

    std::vector<std::string> _lines;

    Response();

    Response(std::string data);

    Response(ResponseCode statusCode);

    bool AddResonseCode(ResponseCode statusCode);

    bool AddLine(std::string line);
};