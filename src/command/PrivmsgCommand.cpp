#include <vector>
#include "Channel.hpp"
#include "Client.hpp"
#include "Command.hpp"
#include "PrivmsgCommand.hpp"

PrivmsgCommand::PrivmsgCommand(Server *server, const int clientFd, Command::t_msgSpecs specs, const std::vector<std::vector<std::string> > params) : Command(server, clientFd, specs, params)
{
	if (!server->getClient(clientFd)->isfullyRegistered())
	{
		throw Command::NotRegisteredException(server->getClientNickname(clientFd) + ":You are not registered");
	}
	if (params.size() < PrivmsgCommand::min_params || this->_trailer.empty())
		throw Command::NotEnoughParametersException("Not enough parameters");
}

PrivmsgCommand::~PrivmsgCommand() {}

void PrivmsgCommand::execute() const
{
	std::vector<std::string> 	targets = this->_params.front();
	std::string 				msgContent = this->_trailer;

	for (size_t i = 0; i < targets.size(); i++)
	{
		std::string const &target = targets[i];

		if (target[0] == '#')
		{
			std::map<std::string, Channel *> const &m = this->_server->getChannelMap();
			if (m.end() == m.find(target))
			{
				std::string reply = this->_director.errNoSuchChannel(this->_client->getNickname(), target);
				this->_server->writeInBuffer(this->getClient(), reply);
				continue ;
			}

			Channel *chan = m.find(target)->second;
			if (chan->isUserInChannel(this->_client->getFd()))
				chan->sendMessageToAllOther(this->_director.rplPrivmsg(*this->_client, *chan, msgContent), this->_client->getFd());
			else
			{
				std::string reply = this->_director.errNotOnChannel(this->_client->getNickname(), target);
				this->_server->writeInBuffer(this->getClient(), reply);
			}
		}
		else
		{
			Client *targetClient = this->_server->getClient(target);
			if (targetClient == NULL)
			{
				std::string reply = this->_director.errNoSuchNick(this->_client->getNickname(), target);
				this->_server->writeInBuffer(this->getClient(), reply);
				continue ;
			}
			std::string reply = this->_director.rplPrivmsg(*this->_client, *targetClient, msgContent);
			this->_server->writeInBuffer(targetClient, reply);
		}
	}
}
