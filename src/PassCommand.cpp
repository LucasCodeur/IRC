#include <sys/socket.h>
#include <iostream>

#include "PassCommand.hpp"
#include "Client.hpp"
#include "Exceptions.hpp"

PassCommand::PassCommand(Server *server, const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > params) : Command(server, clientFd, type, params)
{
	if (params.size() < PassCommand::min_params)
		throw Command::IncorrectParametersException("Not enough parameters");
	else if (params.size() > PassCommand::max_params)
		throw Command::IncorrectParametersException("Too much parameters");
	if (type != PASS)
		throw UnknownCommandException(); //FIXME: use appropriate exception for this
}

PassCommand::~PassCommand() {};

void	PassCommand::execute() const
{
	std::map<int, Client*>::const_iterator	it = _server->getClientmap().find(this->getClientFd());
	std::string								message = "Password correct\n";
	std::string								password = this->_params[0][0];

	if (it->second->authState.getFullyRegistered() == false)
	{
		it->second->authState.setPasswordReceived(true);

		if (password != _server->getPassword())
			throw passwordNotCorrect();

		if (send(this->getClientFd(), message.c_str(), message.size(), 0) < 0)
			throw sendFailed();
	}
	// else
	// 	throw std::runtime_error("The Client still has a password registered.\n");
}
