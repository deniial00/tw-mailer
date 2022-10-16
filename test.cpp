#include <iostream>
#include "src/Message.hpp"

int main(int argc, char const* argv[])
{
    const char* string = "Daniel Hofbauer\nMario Gussnig\nIst das cool?\nTesty\n.\n";
    Message request(string, strlen(string));
    
    std::cout   << "====================" << std::endl 
                << "Message to server: " << std::endl 
                << "====================" << std::endl
                << request.ToString();
    
}