# Create variables
SERVER_CPP := tw-mailer-server.cpp src/ServerController.cpp src/Message.cpp
CLIENT_CPP := tw-mailer-client.cpp src/Message.cpp
HEADER_DIR := src/

# Main target
all: server client

# -I Adds a directory used for header files and libraries
server: $(SERVER_CPP)
	g++ -g -Wall -o tw-server -std=c++11 $(SERVER_CPP) -I $(HEADER_DIR)

client: $(CLIENT_CPP)
	g++ -g -Wall -o tw-client -std=c++11 $(CLIENT_CPP) -I $(HEADER_DIR)

test: test.cpp src/Message.hpp src/Message.cpp
	g++ -g -Wall -o test test.cpp src/Message.cpp 

clean: 
	rm tw-server
	rm tw-client
	rm test