#include <string>
#include <vector>
#include "Message.h"
#include "ResponseCode.h"

class Response : private Message
{
    ResponseCode _statusCode;

    std::vector<std::string> _lines;

    Response();

    Response(char* data);

    Response(ResponseCode statusCode);

    void AddResonseCode(ResponseCode statusCode);

    void AddLine(std::string line);

    char* ToChar();

    void ParseData(char* data);

    void ParseLine();
};