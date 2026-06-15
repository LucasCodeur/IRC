#include <cstddef>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>

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
bool	receiveData(int serverSocket, std::string& serverBuffer);
bool	handleRequest(int serverSocket, std::string& serverBuffer);

int	main(int argc, char* argv[])
{
	(void)argv;

	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
		return 2;
	}

	signal(SIGINT, signalHandler);
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in serverAddress;
	int port = 0;
	convertPort(argv[1], port);

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	connect(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

	std::string message = "PASS ";
	message += argv[2];
	message += "\r\n";	
	message += "NICK botIrc\r\n";
	message += "USER botIrc botIrc 0 :bot_server\r\n";
	send(serverSocket, message.c_str(), strlen(message.c_str()), 0);
	std::string serverBuffer;
	while (stopVar == false)
	{
		try 
		{
			if (receiveData(serverSocket, serverBuffer) == true)
				handleRequest(serverSocket, serverBuffer);
		}
		catch (std::exception &e)
		{
			continue ;
		}
	}

	close(serverSocket);

	return (0);
}

static std::string	extractCommand(std::string& buffer);

bool	handleRequest(int serverSocket, std::string& serverBuffer)
{
	bool		stop = false;

	while (!stop)
	{
		std::string		strCommand;

		strCommand = extractCommand(serverBuffer);
		if (strCommand.empty())
		{
			stop = true;
			continue ;
		}
		std::cout << "JUST BEFORE TO SEND WELCOME TO THE SERVER" << std::endl;
		std::string		message = "WELCOME ";
		size_t pos = strCommand.find(" ");
		std::string command;	
		if (pos != std::string::npos)
		{
			command = strCommand.substr(0, pos);
			strCommand.erase(0, pos);
		}
		if (command == "SEND")
		{
			message += strCommand;
			message += "\r\n";
			if (send(serverSocket, message.c_str(), strlen(message.c_str()), 0) < 0)
			{
				std::cout << "send failed" << std::endl;
				return (false);
			}
		}
	}
	return (true);
}

bool	receiveData(int serverSocket, std::string& serverBuffer)
{
	int		bytes_read;
	char	buffer[BUFFER_SIZE] = {"0"};

	bytes_read = recv(serverSocket, buffer, sizeof(buffer), 0);
	if (bytes_read <= 0)
	{
		if (bytes_read == 0 || ((bytes_read == -1) && (errno != EAGAIN && errno != EWOULDBLOCK)))
			return (false);
	}
	buffer[bytes_read] = '\0';
	serverBuffer += buffer;
	std::cout << "serverBuffer" << std::endl;
	std::cout << serverBuffer << std::endl;
	return (true);
}

void signalHandler(int signum)
{
	(void)signum;
	std::cerr << "shutting down server..." << std::endl;
	stopVar = true;
}

/**
 * @brief function to extract a valid command from the buffer.
 * @param buffer, string to extract the command.
 * @return a valid command.
 */
static std::string	extractCommand(std::string& buffer)
{
	std::string		res;
	size_t			pos = buffer.find("\n");

	if (pos != std::string::npos)
	{
		res = buffer.substr(0, pos);
		buffer.erase(0, pos + 1);
	}
	int size = res.size();
	if (res[size] != '\n' && res[size - 1] != '\r')
		throw std::runtime_error("Not carriage or newline at the end of the command");
	res = res.substr(0, size - 1);
	return (res);
}
