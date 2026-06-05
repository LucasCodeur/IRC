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
	if (!this->_trailer.empty())
		this->_newTopic = this->_trailer;
}

TopicCommand::~TopicCommand() {}

void TopicCommand::executeTopicChange(Channel *distTargetChannel) const
{
	std::string reply;

	if (!distTargetChannel->isOp(this->getClientFd()) && distTargetChannel->isTopicRestricted())
	{
		reply = this->_director.errChanOPrivsNeeded(this->getClient()->getNickname(), this->_targetChannel);
		this->_server->sendData(this->getClientFd(), reply);
		return ;
	}

	distTargetChannel->setTopic(this->_newTopic);
	reply = this->_director.rplTopic(this->getClient()->getNickname(), this->_targetChannel, distTargetChannel->getTopic());
	this->_server->sendData(this->getClientFd(), reply);
}

void TopicCommand::execute() const
{
	std::string reply;

	std::map<std::string, Channel *> channels = _server->getChannelMap();
	Channel *distTargetChannel = channels.find(this->_targetChannel)->second;

	if (distTargetChannel == NULL || !distTargetChannel->isOnChan(this->getClientFd()))
	{
		reply = this->_director.errNotOnChannel(this->getClient()->getNickname(), this->_targetChannel);
		this->_server->sendData(this->getClientFd(), reply);
		return ;
	}

	if (!this->_newTopic.empty())
	{
		this->executeTopicChange(distTargetChannel);
		return ;
	}

	if (distTargetChannel->getTopic().empty())
	{
		reply = this->_director.rplNoTopic(this->getClient()->getNickname(), this->_targetChannel);
		this->_server->sendData(this->getClientFd(), reply);
		return ;
	}

	reply = this->_director.rplTopic(this->getClient()->getNickname(), this->_targetChannel, distTargetChannel->getTopic());
	this->_server->sendData(this->getClientFd(), reply);
	return ;

}
