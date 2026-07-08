#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include <iostream>
#include <ctime> 
#include <cstddef>
#include <cstring>
#include <iostream>

#include "Bot.hpp"
#include "utils.hpp"

int stopVar = false;

# define BUFFER_SIZE 4096
# define CRLN "\r\n"

void	Bot::launcher_bot(std::string strPort, std::string password, std::string channel)
{
	try
	{
		this->_socketServer = socket(AF_INET, SOCK_STREAM, 0);

		int port = 0;
		utils_server::convertPort(strPort, port);

		utils_server::setAddr(this->_serverAddress, port);

		this->connectToServer();

		utils_server::setNonBlocking(this->_socketServer);
		this->sendConnectionToServer(password, channel);
		while (stopVar == false)
		{
			if (utils_server::receiveData(this->_socketServer, this->_buf) == true)
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

static bool	splitPrivmsg(std::string strCommand, std::string& nick, std::string& content);
static std::string getTimeString();

bool	Bot::handleRequest()
{
	while (stopVar == false)
	{
		std::string		strCommand;

		strCommand = utils_server::extractCommand(this->_buf, false);
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

void	Bot::sendConnectionToServer(std::string password, std::string channel)
{
	std::string message = "PASS ";
	message += password;
	message += CRLN;

	utils_server::sendData(this->_socketServer, message);
	message = "NICK botIrc\r\n";
	utils_server::sendData(this->_socketServer, message);
	message = "USER botIrc botIrc 0 :bot_server\r\n";
	utils_server::sendData(this->_socketServer, message);
	message = "JOIN " + channel; 
	message += CRLN;
	utils_server::sendData(this->_socketServer, message);
}

static std::string getTimeString();

static bool	splitPrivmsg(std::string strCommand, std::string& nick, std::string& content)
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

void	sendPrivateMessage(int socket, std::string nick, std::string content)
{
	std::string message = "PRIVMSG " + nick + " :" + content;
	message += CRLN;
	utils_server::sendData(socket, message);
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

void	Bot::connectToServer()
{
	if (connect(this->_socketServer, (struct sockaddr*)&this->_serverAddress, sizeof(this->_serverAddress)) < 0)
		throw(std::runtime_error("connect failed"));
}

void	Bot::setSocket(int socket)
{
	this->_socketServer = socket;
}

Bot::~Bot()
{
	_nicks.clear();
}

void signalHandler(int signum)
{
	(void)signum;
	std::cerr << "shutting down bot..." << std::endl;
	stopVar = true;
}
