#include "Command.hpp"
#include "debug.hpp"
#include <iostream>

Command::Command()
	: _clientFd(-1),
	  _commandType(EMPTY)
{
	// std::cout << GREEN "Command created: " RESET << *this <<std::endl;
}

Command::Command(int clientFd, Command::commandType type, std::vector<std::string> const &params)
	: _clientFd(clientFd),
	  _commandType(type),
	  _params(params)
{
	// std::cout << GREEN "Command created: " RESET << *this <<std::endl;
}

Command::~Command()
{
	// std::cout << RED "Command destroyed: " RESET << *this <<std::endl;
}

Command::Command(Command const &original)
	: _clientFd(original._clientFd),
	  _commandType(original._commandType),
	  _params(original._params)
{
	// std::cout << BLUE "Command copied: " RESET << *this <<std::endl;
}

Command &Command::operator=(Command const &other)
{
	if (this != &other)
	{
		this->_clientFd = other._clientFd;
		this->_commandType = other._commandType;
		this->_params = other._params;
		// std::cout << BLUE "Command assigned: " RESET << *this << std::endl;
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

	o << "Command: " << obj.getClientFd() << ", " << obj.getCommandType() << ", [";
	for (size_t i = 0; i < params.size(); ++i)
	{
		o << params[i];
		if (i < params.size() - 1)
			o << ", ";
	}
	o << "]";
	return (o);
}
