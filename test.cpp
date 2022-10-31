#include <iostream>
#include "src/Message.hpp"
#include "src/ServerController.hpp"

int main(int argc, char const* argv[])
{
    const char* string = "Daniel Hofbauer\nMario Gussnig\nIst das cool?\nTesty\n.\n";
    Message mess(string, strlen(string));
    
    auto server = new ServerController(8080,"/data");

    server->StoreMessageToDir(mess, "Daniel Hofbauer");
    
}