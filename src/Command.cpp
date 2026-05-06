#include "Command.hpp"
#include "debug.hpp"
#include <iostream>
#include <sstream>
#include <stdexcept>

Command::Command()
	: _clientFd(-1),
	  _commandType(EMPTY)
{
	if (DEBUG == 1)
		std::cout << DBUG GREEN "Command created: " RESET << *this <<std::endl;
}

Command::Command(int clientFd, std::string str) : _clientFd(clientFd)
{
	const char *types[11] = {"JOIN", "PRIVMSG", "KICK", "INVITE", "TOPIC", "MODE", "WHO", "PASS", "NICK", "USER", "PART"};

	std::vector<std::string> commandTypes(types, types + COMMAND_TYPES_AMOUNT);
	std::vector<std::string> formattedCommand;
	std::stringstream ss(str);


	std::string splitString;
	while(getline(ss, splitString, ' '))
		formattedCommand.push_back(splitString);
	if (formattedCommand.size() <= 0)
		throw EmptyCommandException();
	std::vector<std::string> parameters(++formattedCommand.begin(), formattedCommand.end());
	
	for (size_t	i = 0; i < commandTypes.size(); ++i)
	{
	  // std::cout << commandTypes[i] << " vs " << formattedCommand[0] << std::endl;
		if (formattedCommand[0] == commandTypes[i])
		{
			std::cout << "identified type : " << commandTypes[i] << std::endl;
			this->_commandType = (commandType) i;
			break ;
		}
		if (i == commandTypes.size() - 1)
			throw UnknownCommandException();
	}

	this->_params = parameters;
	}


Command::Command(int clientFd, Command::commandType type, std::vector<std::string> const &params)
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

// EXCEPTIONS
Command::UnknownCommandException::UnknownCommandException() : std::runtime_error("Unknown command type") {}
Command::UnknownCommandException::~UnknownCommandException() throw() {}

Command::EmptyCommandException::EmptyCommandException() : std::runtime_error("Empty command") {}
Command::EmptyCommandException::~EmptyCommandException() throw() {}


Command::IncorrectParametersException::IncorrectParametersException() : std::runtime_error("number of parameters incorrect") {}
Command::IncorrectParametersException::~IncorrectParametersException() throw() {}

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

int Command::getClientFd() const
{
	return (_clientFd);
}

Command::commandType Command::getCommandType() const
{
	return (_commandType);
}

std::vector<std::string> const &Command::getParams() const
{
	return (_params);
}

std::ostream &operator<<(std::ostream &o, const Command &obj)
{
	std::vector<std::string> const &params = obj.getParams();

	o << "Command: clientFd " << obj.getClientFd() << ", " << obj.commandTypeToString() << ", [";
	for (size_t i = 0; i < params.size(); ++i)
	{
		o << params[i];
		if (i < params.size() - 1)
			o << ", ";
	}
	o << "]";
	return (o);
}

void Command::setClientFd(int fd)
{
	this->_clientFd = fd;
}

void Command::setCommandType(commandType type)
{
	this->_commandType = type;
}

void Command::setParams(std::vector<std::string> const &params)
{
	this->_params = params;
}

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
