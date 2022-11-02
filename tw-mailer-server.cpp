#include "src/ServerController.hpp"

int main(int argc, char const* argv[])
{
    if(argc != 3) {
        std::cout << "[ERROR] Port or dataStore location not provided" << std::endl;
        exit(EXIT_FAILURE);
    }
    // get params from args
    int port = std::stoi(argv[1]);
    std::string dataStore = (std::string) argv[2];

    // create server and listen
    ServerController* server = new ServerController(port, dataStore);
    server->Listen();
}
