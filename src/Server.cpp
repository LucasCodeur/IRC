#include "Server.hpp"
#include "debug.hpp"
#include <iostream>
#include <string>
#include <vector>

Server::Server()
	: _port(0),
	  _fd(-1),
	  _serverName("ircserv"),
	  _password("")
{
	if (DEBUG == 1)
		std::cout << DBUG GREEN "Server created: " RESET << *this << std::endl;
}

Server::Server(int port, const std::string &password)
	: _port(port),
	  _fd(-1),
	  _serverName("ircserv"),
	  _password(password)
{
	if (DEBUG == 1)
		std::cout << DBUG GREEN "Server created: " RESET << *this << std::endl;
} 

Server::~Server()
{
	for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
		delete it->second;
	if (DEBUG == 1)
		std::cout << DBUG RED "Server destroyed: " RESET << *this << std::endl;
}

Server::Server(Server const &original)
	: _port(original._port),
	  _fd(original._fd),
	  _serverName(original._serverName),
	  _password(original._password),
	  _clients(original._clients)
{
	if (DEBUG == 1)
		std::cout << DBUG BLUE "Server copied: " RESET << *this << std::endl;
}

Server &Server::operator=(Server const &other)
{
	if (this != &other)
	{
		this->_port = other._port;
		this->_fd = other._fd;
		this->_serverName = other._serverName;
		this->_password = other._password;
		this->_clients = other._clients;
		if (DEBUG == 1)
			std::cout << DBUG BLUE "Server assigned: " RESET << *this << std::endl;
	}
	return (*this);
}

int Server::getPort() const
{
	return (this->_port);
}

int Server::getFd() const
{
	return (this->_fd);
}

std::string const &Server::getServerName() const
{
	return (this->_serverName);
}

std::string const &Server::getPassword() const
{
	return (this->_password);
}

void Server::handleCommand(Command const &cmd)
{
	switch (cmd.getCommandType()) // TODO: will add all the commands later
	{
		case Command::JOIN:
			this->handleJoin(cmd);
			break;
		case Command::EMPTY:
			std::cout << DBUG YELLOW "Received empty command from client " << cmd.getClientFd() << RESET << std::endl;
			break;
		default:
			std::cout << cmd.commandTypeToString() << ": not handled for now !" << std::endl;
			break;
	}
}

void Server::handleJoin(Command const &cmd)
{
	std::vector<std::string> const &params = cmd.getParams();
	std::string const &channelName = params[0];
	std::map<std::string, Channel*>::iterator it = this->_channels.find(channelName);
	
	if (it == this->_channels.end())
		it = this->_channels.insert(std::make_pair(channelName, new Channel(channelName))).first;
	if (it->second->addUser(cmd.getClientFd()))
	{
		if (DEBUG == 1)
			std::cout << DBUG GREEN "Client " << cmd.getClientFd() << " joined channel " << channelName << RESET << std::endl;
	}
	else
		if (DEBUG == 1)
			std::cout << DBUG YELLOW "Client " << cmd.getClientFd() << " can't join channel " << channelName << " allready in it" << RESET << std::endl;
}

std::ostream &operator<<(std::ostream &o, const Server &obj)
{
	return (o << "Server name: " << obj.getServerName()
			  << " port: " << obj.getPort());
}
