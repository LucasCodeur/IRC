#include "QuitCommand.hpp"
#include "debug.hpp"
#include <sys/epoll.h>
#include <unistd.h>
#include <iostream>

QuitCommand::QuitCommand(Server *server, const int clientFd, t_msgSpecs specs, const std::vector <std::vector<std::string> > params) : Command(server, clientFd, specs, params)
{
	if (DEBUG)
		std::cout << "	creating QuitCommand" << std::endl;
}

QuitCommand::~QuitCommand()
{
	if (DEBUG)
		std::cout << " QuitCommand destroyed" << std::endl;
}

void	QuitCommand::execute() const
{
	if (DEBUG)
		std::cout << "	Executing QuitCommand" << std::endl;
	std::string reason;
	if (this->_trailer.empty())
		reason = "Client Quit";
	else
		reason = this->_trailer;
	std::string quitMessage = this->_director.rplQuit(*this->getClient(), reason);
	std::map<std::string, Channel *> channels = this->getServer()->getChannelMap();

	for (std::map<std::string, Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		Channel *curChannel = (it->second);
		if (curChannel->isUserInChannel(this->getClientFd()))
		{
			curChannel->sendMessageToAllOther(this->_server, quitMessage, this->getClientFd());
		}
	}
	this->getServer()->removeClient(this->getClientFd());
}
