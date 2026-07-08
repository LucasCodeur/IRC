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

/**
* @brief method allowing to launch the bot
* @param strPort port of the server.
* @param password of the server.
* @param channel where the bot has to connect.
* @return
*/
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

/**
 * @brief allows handling of the client request.
 * @return false if we have to stop or true if we have to continue.
 */
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

/**
* @brief Allows the bot to send a request to the server in order to connect to it.
* @param password send a password to the server in order to connect to it.
* @param channel where the bot must to go.
* @return
*/
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

/**
* @brief Function to split the privmsg received and handle it.
* @param strCommand, message receive by a client.
* @param nick who sent the message.
* @param content only the content of the message.
* @return false if nothing receive or true if yes.
*/
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

/**
* @brief function to send a priv message.
* @param socket, the recipient of the message.
* @param nick, who we will send the message.
* @param content, the content of the message.
*/
void	sendPrivateMessage(int socket, std::string nick, std::string content)
{
	std::string message = "PRIVMSG " + nick + " :" + content;
	message += CRLN;
	utils_server::sendData(socket, message);
}

/**
* @brief allows getting the time.
* @return a string with the date.
*/
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
* @brief allows connecting with the server.
* @return
*/
void	Bot::connectToServer()
{
	if (connect(this->_socketServer, (struct sockaddr*)&this->_serverAddress, sizeof(this->_serverAddress)) < 0)
		throw(std::runtime_error("connect failed"));
}

/**
* @brief setter socket bot.
* @param socket has to be set.
* @return
*/
void	Bot::setSocket(int socket)
{
	this->_socketServer = socket;
}

/**
* @brief destructor of the server, clears the nicks.
* @return
*/
Bot::~Bot()
{
	_nicks.clear();
}

/**
* @brief handles signals in the program.
* @param signum not use.
* @return
*
*/
void signalHandler(int signum)
{
	(void)signum;
	std::cerr << "shutting down bot..." << std::endl;
	stopVar = true;
}
