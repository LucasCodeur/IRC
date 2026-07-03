#include "KickCommand.hpp"
#include "Command.hpp"
#include <iostream>

KickCommand::KickCommand(Server *server, const int clientFd, t_msgSpecs specs, const std::vector<std::vector<std::string> > params) : Command(server, clientFd, specs, params)
{
	if (!server->getClient(clientFd)->isfullyRegistered())
	{
		throw Command::NotRegisteredException(server->getClientNickname(clientFd) + ":You are not registered");
	}
	if (params.size() < KickCommand::min_params)
		throw Command::NotEnoughParametersException("Not enough parameters");
	else if (params.size() > KickCommand::max_params)
		throw Command::NotEnoughParametersException("Too much parameters");
	
	std::vector<std::string> channels = params.front();
	std::vector<std::string> users = params.back();

	if (channels.size() != users.size())
		throw Command::NotEnoughParametersException("KICK");
}

KickCommand::~KickCommand() {}

void KickCommand::execute() const
{
	
	Channel		*currentChannel;
	Client		*currentUser;
	std::string	kickComment = this->_trailer;
	std::string	reply;

	for (unsigned int i = 0; i < this->_params.front().size(); ++i)
	{
		currentChannel = this->getServer()->getChannelByName(this->_params.front()[i]);
		currentUser = this->getServer()->getClient(this->_params.back()[i]);

		if (currentUser == NULL)
			return;


		if (this->_trailer.empty())
			kickComment = "No reason specified";

		std::cerr << "kicking" << currentUser->getNickname() << " from " << currentChannel->getName() << " :" << kickComment << std::endl;

		if (currentChannel == NULL)
		{
			reply = this->_director.errNoSuchChannel(this->getClient()->getNickname(), currentChannel->getName());
			this->getServer()->writeInBuffer(this->getClient(), reply);
			continue ;
		}	

		if (!currentChannel->isUserInChannel(this->getClientFd()))
		{
			reply = this->_director.errNotOnChannel(this->getClient()->getNickname(), currentChannel->getName());
			this->getServer()->writeInBuffer(this->getClient(), reply);
			continue ;
		}
		
		if (!currentChannel->isOp(this->getClientFd()))
		{
			reply = this->_director.errChanOPrivsNeeded(this->getClient()->getNickname(), currentChannel->getName());
			this->getServer()->writeInBuffer(this->getClient(), reply);
			continue ;
		}

		currentChannel->sendMessageToAll(":" + this->getClient()->getNickname() + " KICK " + currentUser->getNickname() + " : " + kickComment);
		currentChannel->sendMessageToAll("hey");
		currentChannel->removeUser(currentUser);

	  std::cout << "kicked " << currentUser->getNickname() << " from " << currentChannel->getName() << " : " << kickComment << std::endl;

	}
}
