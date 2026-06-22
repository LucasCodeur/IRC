#include "Command.hpp"
#include "UserCommand.hpp"
#include "ReplyBuilder.hpp"
#include <iostream>
//WARN: take off
#include <debug.hpp>

UserCommand::UserCommand(Server *server, const int clientFd, Command::t_msgSpecs specs, const std::vector<std::vector<std::string> > params) : Command(server, clientFd, specs, params)
{
	if (params.size() < UserCommand::min_params || this->_trailer.empty())
		throw Command::IncorrectParametersException("Not enough parameters");
	else if (params.size() > UserCommand::max_params)
		throw Command::IncorrectParametersException("Too much parameters");
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
		client->addToBuffer(reply);
		return ;
	}
	if (this->getServer()->getPassword().empty() == true)
		authstate.setPasswordReceived(true);
	if (authstate.getNickReceived() == true && authstate.getPasswordReceived() == true)
	{
		authstate.setFullyRegistered(true);
		client->setUsername(this->_params[0][0]);
		client->setHostname(this->_params[1][0]);
		client->setRealname(this->_trailer);

		std::string clientNickname = client->getNickname();

		if (client->getHostname() != "botIrc")
		{
			try 
			{
				std::string reply;

				reply = this->_director.rplWelcome(clientNickname);
				client->addToBuffer(reply);

				reply = this->_director.rplYourhost(clientNickname);
				client->addToBuffer(reply);

				reply = this->_director.rplCreated(clientNickname);
				client->addToBuffer(reply);

				reply = this->_director.rplMyInfo(clientNickname);
				client->addToBuffer(reply);
			}
			catch (std::exception& e)
			{
				std::cout << "Caught: " << e.what() << std::endl;
					return ;
			}
		}
		else
			_server->setBotFd(it->second->getFd());
	}
	else
		std::cout << "Nick received or password are false" << std::endl;
}
