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

    Response(StatusCode statusCode);

    bool AddStatusCode(StatusCode statusCode);

    bool AddLine(std::string line);
};