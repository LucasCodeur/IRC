/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TopicCommand.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbarru <kbarru@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 11:55:35 by kbarru            #+#    #+#             */
/*   Updated: 2026/05/11 12:07:00 by kbarru           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "TopicCommand.hpp"
#include "debug.hpp"
#include "NumericReplies.h"
#include <iostream>
#include <sstream>

TopicCommand::TopicCommand(const int clientFd, const enum Command::commandType type, const std::vector <std::vector<std::string> > params) : Command(clientFd, type, params)
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

void TopicCommand::execute(Server &server) const
{
	std::ostringstream responseStream;

	std::map<std::string, Channel *> channels = server.getChannelMap();
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
		responseStream << RPL_NOTOPIC << this->_targetChannel << ":No topic is set" << LF << CR ;
	}
	else
	{
		if (!distTargetChannel->isOp(this->getClientFd()))
		{
			responseStream << ERR_CHANOPRIVSNEEDED << this->_targetChannel << ":You're not channel operator" << LF << CR ;
			server.sendData(this->getClientFd(), responseStream.str());
		}
		else
		{
			distChanIt->second->setTopic(this->_newTopic);
			responseStream << RPL_TOPIC << this->_targetChannel << ":" << distTargetChannel->getTopic() << LF << CR;
		}

	}
	server.sendData(this->getClientFd() , responseStream.str());
}
