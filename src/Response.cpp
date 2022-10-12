#include "Response.h"

Response::Response() { }

Response::Response(char* data)
{
    // Parse data into object
}

Response::Response(ResponseCode statusCode)
{
    AddResonseCode(statusCode);
}

void Response::AddResonseCode(ResponseCode statusCode)
{
    try {
        _statusCode = statusCode;
    }
    catch (...){
        throw "Incorrect statusCode"; 
    }
}


void Response::AddLine(std::string line)
{
    _lines.push_back(line);
}

