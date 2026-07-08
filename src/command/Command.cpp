#include "Command.hpp"
#include "Server.hpp"
#include <stdexcept>
/* === CONSTRUCTOR === */

Command::Command(Server *server, int clientFd, Command::t_msgSpecs specs , std::vector<std::vector<std::string> > const &params)
	:
	  _clientFd(clientFd),
	  _params(params),
	  _server(server),
	  _prefix(specs.prefix),
	  _command(specs.command),
	  _trailer(specs.trailer)
{
	this->_client = server->getClient(clientFd);
}

Command::~Command()
{
	// std::cout << RED "Destroying Command" RESET << std::endl;
}

Command::Command(Command const &original)
	:
	  _clientFd(original._clientFd),
	  _params(original._params),
	  _server(original._server)
{
}

/* === OPERATOR OVERRIDE === */

Command &Command::operator=(Command const &other)
{
	if (this != &other)
	{
		this->_server = other._server;
		this->_clientFd = other._clientFd;
		this->_params = other._params;

	}
	return (*this);
}

/* === GETTERS === */

int Command::getClientFd() const
{
	return (_clientFd);
}

Client *Command::getClient() const
{
	return (this->_client);
}

std::vector<std::vector<std::string> > const &Command::getParams() const
{
	return (_params);
}

Server *Command::getServer() const
{
	return (this->_server);
}

/* === SETTERS === */

void Command::setClientFd(int fd)
{
	this->_clientFd = fd;
}


void Command::setParams(std::vector<std::vector<std::string> > const &params)
{
	this->_params = params;
}

/* === EXCEPTIONS === */

Command::UnknownCommandException::UnknownCommandException() : std::runtime_error("Unknown command type") {}
Command::UnknownCommandException::UnknownCommandException(std::string msg) : std::runtime_error(msg) {}
Command::UnknownCommandException::~UnknownCommandException() throw() {}

Command::EmptyCommandException::EmptyCommandException() : std::runtime_error("Empty command") {}
Command::EmptyCommandException::~EmptyCommandException() throw() {}


Command::IncorrectParametersException::IncorrectParametersException() : std::runtime_error("number of parameters incorrect") {}
Command::IncorrectParametersException::IncorrectParametersException(std::string msg) : std::runtime_error(msg) {}
Command::IncorrectParametersException::~IncorrectParametersException() throw() {}

Command::NotEnoughParametersException::NotEnoughParametersException() : std::runtime_error("Need more params") {}
Command::NotEnoughParametersException::NotEnoughParametersException(std::string msg) : std::runtime_error(msg) {}
Command::NotEnoughParametersException::~NotEnoughParametersException() throw() {}

Command::NotRegisteredException::NotRegisteredException() : std::runtime_error("You are not registered") {}
Command::NotRegisteredException::NotRegisteredException(std::string msg) : std::runtime_error(msg) {}
Command::NotRegisteredException::~NotRegisteredException() throw() {}