all: server client

server: tw-mailer-server.cpp 
	g++ -g -Wall -o tw-server tw-mailer-server.cpp 

client: tw-mailer-client.cpp
	g++ -g -Wall -o tw-client tw-mailer-client.cpp 

test: test.cpp
	g++ -g -Wall -o test test.cpp

clean: 
	rm tw-server
	rm tw-client