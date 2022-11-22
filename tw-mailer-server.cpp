#include "src/ServerController.hpp"

int main(int argc, char const* argv[])
{
    if(argc != 3) {
        std::cout << "Port or dataStore location not provided" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    int port = std::stoi(argv[1]);
    std::string dataStore = (std::string) argv[2];
    std::string logfile = "./src/logs.txt";
    std::string blacklist = "./src/blacklist.txt";
    ServerController* server = new ServerController(port, dataStore, blacklist, logfile);
    
    server->Listen();
    return 0;
}