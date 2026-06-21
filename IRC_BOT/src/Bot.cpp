#include <cstddef>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <ctime> 
#include <fcntl.h>
#include <iostream>
#include <string.h>
#include <sstream>

#include "Bot.hpp"
#include "utils.hpp"

#define BUFFER_SIZE 2048

int stopVar = false;

void signalHandler(int signum);

void	Bot::launcher_bot(std::string strPort, std::string password, std::string channel)
{
	try
	{
		this->_socketServer = socket(AF_INET, SOCK_STREAM, 0);

		int port = 0;
		utils_server::convertPort(strPort, port);

		this->_serverAdress.sin_family = AF_INET;
		this->_serverAdress.sin_port = htons(port);
		this->_serverAdress.sin_addr.s_addr = INADDR_ANY;

		this->connectToServer();

		this->setNonBlocking(this->_socketServer);
		this->sendConnectionToServer(password, channel);
		while (stopVar == false)
		{
			if (this->receiveData() == true)
			{
				if (this->handleRequest() == false)
					break ;
			}
			else
				break ;
		}
		close(this->_socketServer);
	}
	catch (std::exception &e)
	{
		close(this->_socketServer);
		return ;
	}
}

void signalHandler(int signum)
{
	(void)signum;
	std::cerr << "shutting down server..." << std::endl;
	stopVar = true;
}

Bot::~Bot()
{
	_nicks.clear();
}

/**
* @brief wrapper function to fcntl(), allowing to set up the socket in a non blocking-mode.
* @return
*/
void Bot::setNonBlocking(int sock)
{
	int result;
	int flags;

	flags = ::fcntl(sock, F_GETFL, 0);
	if (flags == -1)
		throw (std::runtime_error("fcntl failed"));
	flags |= O_NONBLOCK;
	result = fcntl(sock , F_SETFL , flags);
	if (result == -1)
	{
		throw (std::runtime_error("fcntl failed"));
	}
}

void	Bot::sendConnectionToServer(std::string password, std::string channel)
{
	std::string message = "PASS ";
	message += password;

	sendData(this->_socketServer, message);
	sendData(this->_socketServer, "NICK botIrc");
	sendData(this->_socketServer, "USER botIrc botIrc 0 :bot_server");
	message = "JOIN " + channel; 
	sendData(this->_socketServer, message);
}

static std::string getTimeString();

bool	splitPrivmsg(std::string strCommand, std::string& nick, std::string& content)
{
	size_t pos = strCommand.find("PRIVMSG");
	if (pos == std::string::npos)
		return (false);
	pos = strCommand.find(" ");
	nick = strCommand.substr(1, pos);
	strCommand.erase(0, pos);
	pos = strCommand.find(":");
	content = strCommand.substr(pos + 1, strCommand.size());
	return (true);
}

bool	Bot::handleRequest()
{
	while (stopVar == false)
	{
		std::string		strCommand;

		strCommand = this->extractCommand(this->_buf);
		if (strCommand.empty())
			return (true);
		std::string content; 
		std::string nick; 
		if (splitPrivmsg(strCommand, nick, content) == true)
		{
			t_board& grid = this->_nicks[nick];
			if (content == "!2048" || (grid.running == true && content.size() == 1))
				this->launch_2048("4", nick, content[0]);
			else if (content == "!hello")
				sendPrivateMessage(this->_socketServer, nick, "Salut " + nick);
			else if (content == "!time")
				sendPrivateMessage(this->_socketServer, nick, "Time is " + getTimeString());
		}
	}
	return (true);
}

void	sendPrivateMessage(int socket, std::string nick, std::string content)
{
	std::string message = "PRIVMSG " + nick + " :" + content;
	sendData(socket, message);
}

static std::string getTimeString()
{
	time_t ts;
	time(&ts);
	std::tm* dt = std::localtime(&ts);

	char buffer[64];
	std::strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", dt);

	return (buffer);
}

/**
 * @brief function to extract a valid command from the buffer.
 * @param buffer, string to extract the command.
 * @return a valid command.
 */
std::string	Bot::extractCommand(std::string& buffer)
{
	std::string		res;
	size_t			pos = buffer.find("\r");

	if (pos != std::string::npos)
	{
		res = buffer.substr(0, pos + 1);
		buffer.erase(0, pos + 2);
	}
	int size = res.size();
	if (res[size] != '\n' && res[size - 1] != '\r')
		throw std::runtime_error("Not carriage or newline at the end of the command");
	res = res.substr(0, size - 1);
	return (res);
}

void	sendData(int socket, std::string message)
{
	message += "\r\n";
	send(socket, message.c_str(), strlen(message.c_str()), 0);
}

void	Bot::display_buffer(std::string& buffer)
{
	std::cout << "INSIDE RECEIVE DATA: serverBuffer: " << std::endl;
	std::cout << buffer << std::endl;
}

void	Bot::connectToServer()
{
	if (connect(this->_socketServer, (struct sockaddr*)&this->_serverAdress, sizeof(this->_serverAdress)) < 0)
		throw(std::runtime_error("connect failed"));
}

void	Bot::setSocket(int socket)
{
	this->_socketServer = socket;
}
