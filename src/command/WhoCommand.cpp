#include "WhoCommand.hpp"
#include "Command.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "ReplyBuilder.hpp"
#include <map>

WhoCommand::WhoCommand(Server *server, const int clientFd, t_msgSpecs specs, const std::vector<std::vector<std::string> > params) : Command(server, clientFd, specs, params)
{
	if (!server->getClient(clientFd)->isfullyRegistered())
	{
		throw Command::NotRegisteredException(server->getClientNickname(clientFd) + ":You are not registered");
	}
	if (params.size() < WhoCommand::min_params)
		throw Command::NotEnoughParametersException("WHO");
}

WhoCommand::~WhoCommand() {}

void WhoCommand::execute() const
{
	Client *requester = this->_server->getClient(this->getClientFd());
	if (!requester) return;

	std::string target = this->_params[0][0];

	if (!target.empty() && target[0] == '#')
	{
		Channel *channel = this->_server->getChannelByName(target);
		if (channel)
		{
			std::vector<Client *> const &users = channel->getUsers();
			for (size_t i = 0; i < users.size(); i++)
			{
				this->_server->writeInBuffer(this->getClient(), this->_director.rplWhoReply(*requester, channel->getName(), *users[i], channel->isOp(users[i]->getFd())));
			}
		}
	}
	else
	{
		std::map<int, Client*> const &clients = this->_server->getClientmap();
		for (std::map<int, Client*>::const_iterator it = clients.begin(); it != clients.end(); ++it)
		{
			if (!it->second)
				continue;
			if (target == "*" || it->second->getNickname() == target)
				this->_server->writeInBuffer(this->getClient(),
					this->_director.rplWhoReply(*requester, "*", *it->second, false));
		}
	}
	this->_server->writeInBuffer(this->getClient(), this->_director.rplEndOfWho(*requester, target));
}
