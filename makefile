# Create variables
SERVER_CPP := tw-mailer-server.cpp src/ServerController.cpp src/Message.cpp 
CLIENT_CPP := tw-mailer-client.cpp src/ClientController.cpp src/Message.cpp
HEADER_DIR := src/

# Main target
all: server client

# -I Adds a directory used for header files and libraries
server: $(SERVER_CPP)
	g++ -g -Wall -o out/tw-server -lldap -llber -std=c++11 $(SERVER_CPP) -I $(HEADER_DIR) -lldap -llber 

client: $(CLIENT_CPP)
	g++ -g -Wall -o out/tw-client -std=c++11 $(CLIENT_CPP) -I $(HEADER_DIR)

test: test.cpp src/Message.cpp src/ServerController.cpp 
	g++ -g -Wall -o out/test -std=c++11 test.cpp src/Message.cpp src/ServerController.cpp -I $(HEADER_DIR)

clean: 
	rm out/tw-server
	rm out/tw-client
	rm out/test