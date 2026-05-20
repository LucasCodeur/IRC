#include <cstddef>
#include <iostream>
#include <map>
#include <vector>
#include "Command.hpp"
#include "debug.hpp"
#include "PartCommand.hpp"
#include "NumericReplies.h"

PartCommand::PartCommand(Server *server, const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > params) : Command(server, clientFd, type, params)
{
	if (params.size() < PartCommand::min_params)
		server->sendData(ERR_NEEDMOREPARAMS, ""); //FIXME: send 461 ERR_NEEDMOREPARAMS to client
	if (type != PART)
		throw UnknownCommandException();
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
				std::cout << DBUG << fd << GREEN " leaving " << it->first << RESET << std::endl;
			else
			{
				std::cout << DBUG << fd << RED " could NOT leave : " << params[i] << " trying to send 442 ERR_NOTONCHANNEL" << RESET << std::endl;
				this->returnErrorReply(ERR_NOTONCHANNEL, it->second->getName(), *this->_server);
			}
				if (it->second->getUsers().empty())
			{
				std::cout << DBUG << "deleting empty channel " << it->first << std::endl;
				this->_server->removeChannel(it->first);
			}
		}
		else
		{
			this->returnErrorReply(403, params[i], *this->_server);
			// std::cout << DBUG << fd << RED " could NOT leave : " << params[i] << " channel not found" << RESET << std::endl; //FIXME: send 403 ERR_NOSUCHCHANNEL to client
		}
	}
}
