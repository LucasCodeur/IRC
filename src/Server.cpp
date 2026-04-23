#include "Server.hpp"
#include <iostream>

Server::Server()
	: _port(0),
	  _fd(-1),
	  _serverName("ircserv"),
	  _password("")
{
	std::cout << "Server created: " << *this << std::endl;
}

Server::~Server()
{
	std::cout << "Server destroyed: " << *this << std::endl;
}

Server::Server(Server const &original)
	: _port(original._port),
	  _fd(original._fd),
	  _serverName(original._serverName),
	  _password(original._password),
	  _clients(original._clients),
	  _channels(original._channels)
{
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
	(void)cmd;
}

std::ostream &operator<<(std::ostream &o, const Server &obj)
{
	return (o << "Server: " << obj.getServerName()
			  << ": " << obj.getPort());
}
