#include <cstddef>
#include <iostream>
#include <map>
#include <vector>
#include "Command.hpp"
#include "debug.hpp"
#include "PartCommand.hpp"

PartCommand::PartCommand(Server *server, const int clientFd, t_msgSpecs specs, const std::vector<std::vector<std::string> > params) : Command(server, clientFd, specs, params)
{
	if (!server->getClient(clientFd)->isfullyRegistered())
	{
		throw Command::NotRegisteredException(server->getClientNickname(clientFd) + ":You are not registered");
	}
	if (params.size() < PartCommand::min_params)
	{
		throw NotEnoughParametersException("PART");
	}
	else if (PartCommand::max_params != 0 && params.size() > PartCommand::max_params)
	{
		throw IncorrectParametersException("PART");
	}
}

PartCommand::~PartCommand() {}

void PartCommand::execute() const
{
	const std::vector<std::string> &params = this->_params.front();

	const std::map<std::string, Channel*> &chanMap = this->_server->getChannelMap();

	const int fd = this->getClientFd();

	for (size_t i = 0; i < params.size(); i++)
	{
		std::map<std::string, Channel*>::const_iterator it = chanMap.find(params[i]);
		if (it != chanMap.end())
		{
			if (it->second->removeUser(fd))
			{
				std::cout << DBUG << fd << GREEN " leaving " << it->first << RESET << std::endl;
				std::string reason;
				if (this->_trailer.empty())
					reason = "Leaving";
				else
					reason = this->_trailer;
				std::string reply = this->_director.rplPart(*this->_server->getClient(fd), *it->second, reason);
				it->second->sendMessageToAll(reply);
				this->_server->writeInBuffer(this->getClient(), reply);
				if (it->second->getUsers().empty())
				{
					std::cout << RED << "deleting empty channel " << it->first << RESET << std::endl;
					this->_server->removeChannel(it->first);
				}
			}
			else // if user was not on channel
			{
				std::string reply = this->_director.errNotOnChannel(this->getClient()->getNickname(), it->second->getName());
				this->_server->writeInBuffer(this->getClient(), reply);
			}
		}
		else // if channel does not exist
		{
			std::string reply = this->_director.errNoSuchChannel(this->getClient()->getNickname(), params[i]);
			this->_server->writeInBuffer(this->getClient(), reply);
		}
	}
}
	
