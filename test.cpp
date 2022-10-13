#include <iostream>
#include "src/Request.h"

int main(int argc, char const* argv[])
{
    std::string str = "SEND\n";
    
    Request req(str.c_str());

    char* data = req.ToChar();

    std::cout << data;
}