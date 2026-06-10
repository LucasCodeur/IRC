#include <sys/socket.h>

#include "PassCommand.hpp"
#include "Client.hpp"
#include "Exceptions.hpp"
//WARN: take off
#include <debug.hpp>

PassCommand::PassCommand(Server *server, const int clientFd, Command::t_msgSpecs specs, const std::vector<std::vector<std::string> > params) : Command(server, clientFd, specs, params)
{
	if (params.size() < PassCommand::min_params)
		throw NotEnoughParametersException("PASS");
	else if (params.size() > PassCommand::max_params)
		throw Command::IncorrectParametersException("Too much parameters");
}

PassCommand::~PassCommand() {};

void	PassCommand::execute() const
{
	if (this->getServer()->getPassword().empty() == true)
		return ;

	std::map<int, Client*>::const_iterator	it = _server->getClientmap().find(this->getClientFd());
	Client*									client = it->second;
	Authstate&								authstate = client->getAuthstate();

	if (authstate.getFullyRegistered() == true)
	{
		std::string reply;
		reply = this->_director.errAlreadyRegistred();
		if (send(this->getClientFd(), reply.c_str(), reply.size(), 0) < 0)
			throw sendFailed();
		return ;
	}
	std::string								message = "Password correct\n";
	std::string								password = this->_params[0][0];

	if (password != _server->getPassword())
		throw passwordNotCorrect();

	if (send(this->getClientFd(), message.c_str(), message.size(), 0) < 0)
		throw sendFailed();
	
	authstate.setPasswordReceived(true);
}
