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
	std::cout << DEBUG GREEN "Server created: " RESET << *this << std::endl;
}

Server::~Server()
{
	std::cout << DEBUG RED "Server destroyed: " RESET << *this << std::endl;
}

Server::Server(Server const &original)
	: _port(original._port),
	  _fd(original._fd),
	  _serverName(original._serverName),
	  _password(original._password),
	  _clients(original._clients),
	  _channels(original._channels)
{
	std::cout << DEBUG BLUE "Server copied: " RESET << *this << std::endl;
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
		this->_channels = other._channels;
		std::cout << DEBUG BLUE "Server assigned: " RESET << *this << std::endl;
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
		default:
			break;
	}
}

void Server::handleJoin(Command const &cmd)
{
	std::cout << DEBUG "handleJoin called"<< std::endl;
	if (cmd.getParams().empty())
	{
		std::cout << DEBUG RED "JOIN command missing parameters" RESET << std::endl;
		return;
	}
	if (cmd.getParams().size() > 1)
	{	
		std::cout << DEBUG RED "JOIN command has too many parameters" RESET << std::endl;
		return;
	}
	if (cmd.getParams()[0][0] != '#') 
	{
		std::cout << DEBUG RED "JOIN command invalid channel name" RESET << std::endl;
		return;
	}
	if (this->_channels.find(cmd.getParams()[0]) == this->_channels.end())
	{
		this->_channels.insert(std::make_pair(cmd.getParams()[0], Channel(cmd.getParams()[0])));
		std::cout << DEBUG GREEN "Channel created: " RESET << cmd.getParams()[0] << std::endl;
	}
	this->_channels[cmd.getParams()[0]].addUser(cmd.getClientFd());
	std::cout << DEBUG GREEN "Client " << cmd.getClientFd() << " joined channel " << cmd.getParams()[0] << RESET << std::endl;
}

std::ostream &operator<<(std::ostream &o, const Server &obj)
{
	return (o << "Server name: " << obj.getServerName()
			  << " port: " << obj.getPort());
}
