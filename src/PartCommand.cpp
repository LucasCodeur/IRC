#include <cstddef>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include "Command.hpp"
#include "debug.hpp"
#include "PartCommand.hpp"

PartCommand::PartCommand(Server *server, const int clientFd, t_msgSpecs specs, const std::vector<std::vector<std::string> > params) : Command(server, clientFd, specs, params)
{
	if (params.size() < PartCommand::min_params)
	{
		std::string reply = this->_director.errNeedMoreParams(this->getClient()->getNickname(), "PART");
		this->_server->sendData(this->getClientFd(), reply);
	}
	else if (params.size() > PartCommand::max_params)
	{
		throw IncorrectParametersException("Too much parameters in PART"); // TODO: maybe just ignore extra parameters instead of throwing error
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
				std::stringstream ss;
				ss << ":" << this->_server->getClientNickname(fd) << " PART " << it->first << "\r\n";
				send(fd, ss.str().c_str(), ss.str().size(), 0); //TODO: change after the builder is done
				it->second->sendMessageToAll(ss.str()); //TODO: change after the builder is done
				if (it->second->getUsers().empty())
				{
					std::cout << DBUG << "deleting empty channel " << it->first << std::endl;
					this->_server->removeChannel(it->first);
				}
			}
			else // if user was not on channel
			{
				std::string reply = this->_director.errNotOnChannel(this->getClient()->getNickname(), it->second->getName());
				this->_server->sendData(this->getClientFd(), reply);
			}
		}
		else // if channel does not exist
		{
			std::string reply = this->_director.errNoSuchChannel(this->getClient()->getNickname(), it->second->getName());
			this->_server->sendData(this->getClientFd(), reply);
		}
	}
}
