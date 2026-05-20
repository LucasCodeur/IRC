#include "Command.hpp"
#include "debug.hpp"
#include "Server.hpp"
#include "NumericReplies.h"
#include <iostream>
#include <sstream>
#include <stdexcept>

/* === CONSTRUCTOR === */

Command::Command()
	: _clientFd(-1),
	  _commandType(EMPTY)
{
	if (DEBUG == 1)
		std::cout << DBUG GREEN "Command created: " RESET << *this <<std::endl;
}

Command::Command(int clientFd, Command::commandType type, std::vector<std::vector<std::string> > const &params)
	: _clientFd(clientFd),
	  _commandType(type),
	  _params(params)
{
	if (DEBUG == 1)
		std::cout << DBUG GREEN "Command created: " RESET << *this <<std::endl;
}

Command::~Command()
{
	if (DEBUG == 1)
		std::cout << DBUG RED "Command destroyed: " RESET << *this <<std::endl;
}

Command::Command(Command const &original)
	: _clientFd(original._clientFd),
	  _commandType(original._commandType),
	  _params(original._params)
{
	if (DEBUG == 1)
		std::cout << DBUG BLUE "Command copied: " RESET << *this <<std::endl;
}

/* === OPERATOR OVERRIDE === */

Command &Command::operator=(Command const &other)
{
	if (this != &other)
	{
		this->_clientFd = other._clientFd;
		this->_commandType = other._commandType;
		this->_params = other._params;
		if (DEBUG == 1)
			std::cout << BLUE "Command assigned: " RESET << *this << std::endl;
	}
	return (*this);
}

std::ostream &operator<<(std::ostream &o, const Command &obj)
{
	std::vector<std::vector<std::string> > const &params = obj.getParams();

	o << "Command: clientFd " << obj.getClientFd() << ", " << obj.commandTypeToString() << ", args : ";
	for (size_t i = 0; i < params.size(); ++i)
	{
		std::cout << "[" ;
		for (size_t j = 0; j < params[i].size(); ++j)
		{
		o << params[i][j];
		if (j < params[i].size() - 1)
			o << ", ";
		}
		o << "]";
		if (i < params.size() - 1)
			o << ", ";
	}

	return (o);
}

/* === GETTERS === */

int Command::getClientFd() const
{
	return (_clientFd);
}

Command::commandType Command::getCommandType() const
{
	return (_commandType);
}

std::vector<std::vector<std::string> > const &Command::getParams() const
{
	return (_params);
}

/* === SETTERS === */

void Command::setClientFd(int fd)
{
	this->_clientFd = fd;
}

void Command::setCommandType(commandType type)
{
	this->_commandType = type;
}

void Command::setParams(std::vector<std::vector<std::string> > const &params)
{
	this->_params = params;
}

void	Command::returnErrorReply(int n, std::string param, Server &server) const
{
    std::stringstream reply;
    reply << n;
    switch (n)
	{
        case 421: // Unknown Command
            reply << param << ": Unknown Command" << "\r\n";
            break;
		case ERR_NOSUCHCHANNEL:
			reply << param << ":No such channel" << "\r\n";
			break;
		case ERR_NOTONCHANNEL:
			reply << param << ": You're not on that channel" << "\r\n";
			break;
		case ERR_NEEDMOREPARAMS:
			reply << param << ": Not enough parameters" << "\r\n";
			break;
        case 431:
            reply << ": No nickname given" << "\r\n";
            break;
		case RPL_NOTOPIC:
		default:
            reply << "Internal server error" << "\r\n";
	}
    server.sendData(this->_clientFd, reply.str());
}

/* === HELPERS === */

std::string Command::commandTypeToString() const
{
	switch (this->_commandType)
	{
		case EMPTY:
			return ("EMPTY");
		case JOIN:
			return ("JOIN");
		case PRIVMSG:
			return ("PRIVMSG");
		case KICK:
			return ("KICK");
		case INVITE:
			return ("INVITE");
		case TOPIC:
			return ("TOPIC");
		case MODE:
			return ("MODE");
		case WHO:
			return ("WHO");
		case PASS:
			return ("PASS");
		case NICK:
			return ("NICK");
		case USER:
			return ("USER");
		case PART:
			return ("PART");
		default:
			return ("UNKNOWN");
	}
}

/* === EXCEPTIONS === */

Command::UnknownCommandException::UnknownCommandException() : std::runtime_error("Unknown command type") {}
Command::UnknownCommandException::~UnknownCommandException() throw() {}

Command::EmptyCommandException::EmptyCommandException() : std::runtime_error("Empty command") {}
Command::EmptyCommandException::~EmptyCommandException() throw() {}


Command::IncorrectParametersException::IncorrectParametersException() : std::runtime_error("number of parameters incorrect") {}
Command::IncorrectParametersException::IncorrectParametersException(std::string msg) : std::runtime_error(msg) {}
Command::IncorrectParametersException::~IncorrectParametersException() throw() {}
