#include "Server.hpp"
#include "Command.hpp"
#include <iostream>
Server::Server()
	: _port(0), _fd(-1), _serverName("ircserv"), _password("")
{
}

Server::~Server()
{
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
	return *this;
}

std::ostream &operator<<(std::ostream &o, const Server &obj)
{
	(void)obj;
	return o;
}

void Server::handleCommand(Command const &cmd)
{
	switch (cmd.getCommandType())
	{
	case Command::EMPTY:
		std::cout << "Handling EMPTY command " << cmd.getClientFd() << std::endl;
		break;
	case Command::JOIN:
		std::cout << "Handling JOIN command " << cmd.getClientFd() << std::endl;
		this->handleJoin(cmd);
		break;
	default:
		std::cout << "Handling unknown command " << cmd.getClientFd() << std::endl;
		break;
	}
}

void Server::test()
{
	Command cmd(42, Command::EMPTY, std::vector<std::string>());
	this->handleCommand(cmd);
	Command cmd2(42, Command::JOIN, std::vector<std::string>());
	this->handleCommand(cmd2);
}