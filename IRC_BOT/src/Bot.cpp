#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "Server.hpp"
#include "Exceptions.hpp"
#include "Command.hpp"
#include <iostream>
#include <csignal>
#include <string.h>
#include <sstream>

int stopVar = false;

void signalHandler(int signum);

bool    convertPort(std::string port, int& portToSet);

int	main(int argc, char* argv[])
{
	(void)argv;

	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
		return 2;
	}

	signal(SIGINT, signalHandler);
	int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in serverAddress;
	int port = 0;
	convertPort(argv[1], port);

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

	std::string message = "PASS ";
	message += argv[2];
	send(clientSocket, message.c_str(), strlen(message.c_str()), 0);

	close(clientSocket);

	return (0);
}

void signalHandler(int signum)
{
	(void)signum;
	std::cerr << "shutting down server..." << std::endl;
	stopVar = true;
}
