/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TopicCommand.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbarru <kbarru@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 11:55:35 by kbarru            #+#    #+#             */
/*   Updated: 2026/05/29 14:22:50 by kbarru           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "TopicCommand.hpp"
#include "NumericReplies.h"
#include <sstream>

TopicCommand::TopicCommand(Server *server, const int clientFd, Command::t_msgSpecs specs, const std::vector <std::vector<std::string> > params) : Command(server, clientFd, specs, params)
{
	if (params.size() < min_params)
		throw IncorrectParametersException("Not enough parameters");
	if (params.size() > max_params)
		throw IncorrectParametersException("Too much parameters in TOPIC");
	if (params.front().size() > 1)
		throw IncorrectParametersException("Only 1 channel is supported in TOPIC command");
	if (params.size() == 2 && params.back().size() > 1)
		throw IncorrectParametersException("Only 1 new topic is supported in TOPIC command");
	this->_targetChannel = params.front().front();
	this->_newTopic = "";
	if (params.size() == 2)
		this->_newTopic = params.back().front();
}

TopicCommand::~TopicCommand() {}

void TopicCommand::execute() const
{
	std::ostringstream responseStream;
	std::string reply;

	std::map<std::string, Channel *> channels = _server->getChannelMap();
	Channel *distTargetChannel = channels.find(this->_targetChannel)->second;

	std::map<std::string, Channel*>::iterator distChanIt = channels.find(this->_targetChannel);
	if (!distTargetChannel->isOnChan(this->getClientFd()))
		responseStream << ERR_NOTONCHANNEL << this->_targetChannel << ":You're not on that channel" << LF << CR;
	else if (distChanIt == channels.end())
	{
		throw IncorrectParametersException("Specified channel does not exist"); // FIXME: should probably just answer error to client instead of throwing
	}
	else if (this->_newTopic.empty())
	{
		reply = this->_director.rplNoTopic(this->_targetChannel);
		this->_server->sendData(this->getClientFd(), reply);
	}
	else
	{
		if (!distTargetChannel->isOp(this->getClientFd()))
		{
			reply = this->_director.errChanOPrivsNeeded(this->_targetChannel);
			this->_server->sendData(this->getClientFd(), reply);
		}
		else
		{
			reply = this->_director.rplTopic(this->_targetChannel, distTargetChannel->getTopic());
			this->_server->sendData(this->getClientFd(), reply);
		}

	}
}
