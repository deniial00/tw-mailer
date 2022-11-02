#include <iostream>
#include "src/Message.hpp"
#include "src/ServerController.hpp"

int main(int argc, char const* argv[])
{
    std::string string = "Daniel Hofbauer\nMario Gussnig\nDas ist ein süsser Test\nJa, der ist wirklich sehr süss..\n.\n";
    Message mess(string, "DanielHofbauerMarioGussnigDasisteinsüsserTest1667331180");
    
    auto server = new ServerController(8080,"./data");

    // auto list = server->GetMessages("Daniel Hofbauer");
    // server->DeleteMessage(mess);
    // std::cout << list.size();

    // std::cout << list.at(0) << std::endl;
    
}