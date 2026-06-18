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

#define BUFFER_SIZE 2048

int stopVar = false;

void signalHandler(int signum);

void	Bot::launcher_bot(std::string strPort, std::string password, std::string channel)
{
	try
	{
		this->_socketServer = socket(AF_INET, SOCK_STREAM, 0);

		int port = 0;
		this->convertPort(strPort, port);

		this->_serverAdress.sin_family = AF_INET;
		this->_serverAdress.sin_port = htons(port);
		this->_serverAdress.sin_addr.s_addr = INADDR_ANY;

		this->connectToServer();

		this->setNonBlocking(this->_socketServer);
		this->sendConnectionToServer(password, channel);
		while (stopVar == false)
		{
			// std::cout << "Inside launcher_bot : " << stopVar << std::endl;

			if (this->receiveData() == true)
			{
				if (this->handleRequest() == false)
					break ;
			}
			else
			{
				// std::cout << "Launcherbot before return "  << std::endl;
				break ;

			}
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
		// std::cout << "Inside handle request: " << stopVar << std::endl;
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
			{
				this->launch_2048("4", nick, content[0]);
			}
			else if (content == "!hello")
				sendPrivateMessage(this->_socketServer, nick, "Salut " + nick);
			else if (content == "!time")
				sendPrivateMessage(this->_socketServer, nick, "Time is " + getTimeString());
			// std::cout << "nick: " << nick << "content: " << content << std::endl;
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

bool	Bot::receiveData()
{
	int	bytes_read;
	char		buffer[BUFFER_SIZE + 1] = {"0"};

	memset(buffer, 0, BUFFER_SIZE);
	bytes_read = recv(this->_socketServer, buffer, BUFFER_SIZE, 0);
	if (bytes_read <= 0)
	{
		if (bytes_read == 0 || ((bytes_read == -1) && (errno != EAGAIN && errno != EWOULDBLOCK)))
			return (false);
	}
	buffer[bytes_read] = '\0';
	if (strlen(buffer) != 0 && buffer[0] != '\0')
	{
		this->_buf += buffer;
		// this->display_buffer(this->_buf);
	}
	return (true);
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
	// if (res[size] != '\n' && res[size - 1] != '\r')
	// 	throw std::runtime_error("Not carriage or newline at the end of the command");
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

static bool check_port(std::string& port);

/**
 * @brief function to check if the port is correct and convert this one.
 * @param port string to convert into number.
 * @return true if the port is correct or false if not the case.
 */
bool    Bot::convertPort(std::string port, int& portToSet)
{
    if (check_port(port) == false)
        throw std::runtime_error("Bad characters inside port");

    std::stringstream ss(port);
    if (ss.fail() == true)
        throw std::runtime_error("Bad characters inside port");

    ss >> portToSet;
    if (1023 >= portToSet || portToSet >= 49152)
        throw std::runtime_error("Bad range of port");

    return (true);
}

/**
 * @brief function to check if only digit inside port.
 * @param port string to check.
 * @return true if correct, false or not.
 */
static bool check_port(std::string& port)
{
    int size = port.size();
    for (int i = 0; i < size; i++)
    {
        if (std::isdigit(port[i]) == false)
            return (false);
    }
    return (true);
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
