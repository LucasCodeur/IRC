/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TopicCommand.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbarru <kbarru@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 11:55:35 by kbarru            #+#    #+#             */
/*   Updated: 2026/05/20 15:22:39 by kbarru           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "TopicCommand.hpp"
#include "NumericReplies.h"
#include <iostream>
#include <sstream>

TopicCommand::TopicCommand(Server *server, const int clientFd, const enum Command::commandType type, const std::vector <std::vector<std::string> > params) : Command(server, clientFd, type, params)
{
	if (params.size() < min_params)
		throw IncorrectParametersException("Not enough parameters");
	if (params.size() > max_params)
		throw IncorrectParametersException("Too much parameters in TOPIC");
	if (type != TOPIC)
		throw UnknownCommandException(); //FIXME: use appropriate exception for this
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
		this->returnErrorReply(RPL_NOTOPIC, "", *_server);
	}
	else
	{
		if (!distTargetChannel->isOp(this->getClientFd()))
		{
			this->returnErrorReply(ERR_CHANOPRIVSNEEDED, "", *_server);
		}
		else
		{
			distChanIt->second->setTopic(this->_newTopic);
			responseStream << RPL_TOPIC << this->_targetChannel << ":" << distTargetChannel->getTopic() << LF << CR;
		}

	}
	_server->sendData(this->getClientFd() , responseStream.str());
}
