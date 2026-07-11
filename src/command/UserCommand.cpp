#include "Command.hpp"
#include "UserCommand.hpp"
#include "ReplyBuilder.hpp"
#include <iostream>
//WARN: take off
#include <debug.hpp>

UserCommand::UserCommand(Server *server, const int clientFd, Command::t_msgSpecs specs, const std::vector<std::vector<std::string> > params) : Command(server, clientFd, specs, params)
{
	if (params.size() < UserCommand::min_params || this->_trailer.empty())
		throw Command::NotEnoughParametersException("Not enough parameters");
	else if (params.size() > UserCommand::max_params)
		throw Command::NotEnoughParametersException("Too much parameters");
}

UserCommand::~UserCommand() {}

void	UserCommand::execute() const
{
	std::map<int, Client*>::const_iterator	it = _server->getClientmap().find(this->getClientFd());
	Client*									client = it->second;
	Authstate&								authstate = client->getAuthstate();

	if (authstate.getFullyRegistered() == true)
	{
		std::string reply;
		reply = this->_director.errAlreadyRegistred();
		this->_server->writeInBuffer(this->_client, reply);
		return ;
	}
	if (this->getServer()->getPassword().empty() == true)
		authstate.setPasswordReceived(true);

	client->setHostname(this->_params[1][0]);
	client->setRealname(this->_trailer);
	client->setUsername(this->_params[0][0]);

	authstate.setUserReceived(true);
	if (authstate.getNickReceived() == true && authstate.getPasswordReceived() == true)
	{
		authstate.setFullyRegistered(true);
		this->_server->sendWelcomePack(*client);
	}
	else
		std::cout << "Nick received or password are false" << std::endl;
}
