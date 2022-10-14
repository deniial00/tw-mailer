#include "Message.hpp"

Message::Message(const char* data)
{
    
}

const char* Message::ToChar()
{
    std::string temp = _sender + "\n"
                     + _receiver + "\n"
                     + _subject + "\n"
                     + _message + "\n.\n";

    return temp.c_str();
}