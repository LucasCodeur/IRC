#include <sys/socket.h>

#include "PassCommand.hpp"
#include "Client.hpp"
#include "Exceptions.hpp"
//WARN: take off
#include <debug.hpp>

PassCommand::PassCommand(Server *server, const int clientFd, Command::t_msgSpecs specs, const std::vector<std::vector<std::string> > params) : Command(server, clientFd, specs, params)
{
	std::map<int, Client*>::const_iterator	it = _server->getClientmap().find(this->getClientFd());
	if (it->second->authState.getFullyRegistered() == true)
			throw NotEnoughParametersException(""); //FIXME: Have to change, maybe throw other things
	if (params.size() < PassCommand::min_params)
			throw NotEnoughParametersException("PASS");
	else if (params.size() > PassCommand::max_params)
		throw Command::IncorrectParametersException("Too much parameters");
}

PassCommand::~PassCommand() {};

void	PassCommand::execute() const
{
	std::map<int, Client*>::const_iterator	it = _server->getClientmap().find(this->getClientFd());
	std::string								message = "Password correct\n";
	std::string								password = this->_params[0][0];

	it->second->authState.setPasswordReceived(true);

	if (password != _server->getPassword())
		throw passwordNotCorrect();

	if (send(this->getClientFd(), message.c_str(), message.size(), 0) < 0)
		throw sendFailed();
}
