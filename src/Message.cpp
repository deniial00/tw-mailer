#include "Message.hpp"


// Message::Message(std::string id)
// {
//     _identifier = id;
// }

Message::Message(const char* data, int length)
{
    int i = 0;
    std::string temp = "";
    while(i <= length)
    {
        temp += data[i];
        i++;
    }
    this->ParseData(temp);
}

Message::Message(std::string data, std::string identifier)
{
    this->ParseData(data);
    this->SetIdentifier(identifier);
}

void Message::ParseData(std::string data)
{
    int lineNum = 0;
    size_t pos = 0;
    std::string line;
    std::string delimiter = "\n";

    while ((pos = data.find(delimiter)) != std::string::npos) {
        line = data.substr(0, pos);

        switch(lineNum)
        {
            case 0:
                this->SetSender(line);
                break;
            case 1:
                this->SetReceiver(line);
                break;
            case 2:
                this->SetSubject(line);
                break;
            default:
                this->_message += line;
                break;
        }

        data.erase(0, pos + delimiter.length());
        lineNum++;
    }
}


std::string Message::ToString()
{
    std::string temp = _sender + "\n" + _receiver + "\n" + _subject + "\n" + _message + "\n.\n";
    
    return temp;
}

void Message::SetSender(std::string sender)
{
    _sender = sender;
}

void Message::SetReceiver(std::string receiver)
{
    _receiver = receiver;
}


int Message::SetSubject(std::string subject)
{
    if(subject.length() > 80)
        return -1;

    _subject = subject;

    return 1;
}

void Message::SetMessage(std::string message)
{
    _message = message;
}

std::string Message::SetIdentifier(std::string identifier)
{
    if(identifier != "") 
    {
        _identifier = identifier;
        return _identifier;
    }

    std::time_t t = std::time(0);  // t is an integer type
    _identifier = _sender + _receiver + _subject + std::to_string(t);

    // remove whitespaces
    _identifier.erase (std::remove (_identifier.begin(), _identifier.end(), ' '), _identifier.end()); 
    
    return _identifier;   
}

std::string Message::GetIdentifier()
{
    return _identifier;
}

std::string Message::GetSender()
{
    return _sender;

}

std::string Message::GetReceiver()
{
    return _receiver;

}

std::string Message::GetSubject()
{
    return _subject;
}

std::string Message::GetMessage()
{
    return _message;
}

std::ostream& operator<< (std::ostream& os, Message& mess) {
    os << "Identifier:" << mess.GetIdentifier() << std::endl
       << "Sender:    " << mess.GetSender() << std::endl
       << "Receiver:  " << mess.GetReceiver() << std::endl
       << "Subject:   " << mess.GetSubject() << std::endl
       << "Message:   " << mess.GetMessage() << std::endl;

    return os;
}

