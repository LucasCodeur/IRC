/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbarru <kbarru@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 13:51:13 by kbarru            #+#    #+#             */
/*   Updated: 2026/05/29 14:43:58 by kbarru           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ModeCommand.hpp"
#include "Command.hpp"
#include "debug.hpp"
#include <iostream>

ModeCommand::ModeCommand(Server *server, const int clientFd, Command::t_msgSpecs specs, const std::vector<std::vector<std::string> > params) : Command(server, clientFd, specs, params)
{
	std::string	modeEdition = "";
	std::string	reply;

	if (params.front().size() < ModeCommand::min_params)
	{
		throw Command::NotEnoughParametersException(this->_command);
	}

	std::string	targetChannelName = params.front().front();

	if (params.size() > 1)
		modeEdition = params[1].front(); // eg. +i

	if (params.size() == 2 && (modeEdition.length() != 2 || (modeEdition[0] != '+' && modeEdition[0] != '-')))
	{
		throw Command::IncorrectParametersException(); //NOTE: maybe just silently ignore?
	}

	std::cout << "Mode edition : " << modeEdition << std::endl;
	if (modeEdition.length() > 1)
	{
		this->_operationChar = params[1][0][0]; // eg. +
		this->_modeChar = params[1][0][1]; // eg. i
	}
	else
	{
		this->_operationChar = "";
		this->_modeChar = "";
	}

		
	if ((this->_modeChar == "k" || this->_modeChar == "o") && this->_params.size() < 3)
	{
		throw Command::NotEnoughParametersException(this->_command);
	}

	this->_targetChannel = this->_server->getChannelByName(targetChannelName);
	if (this->_targetChannel == NULL)
	{
		throw Command::IncorrectParametersException("MODE : No such channel");
	}
}

ModeCommand::~ModeCommand() {}

void ModeCommand::replyChannelMode() const
{
	Director	director;
	std::string	rplParam;
	if (this->_targetChannel->isInviteOnly())
		rplParam += 'i';
	if (this->_targetChannel->isTopicRestricted())
		rplParam += 't';
	if (this->_targetChannel->hasPassword())
		rplParam += 'k';
	if (this->_targetChannel->hasUserLimit())
		rplParam += 'l';
	if (rplParam.length() > 0)
		rplParam = '+' + rplParam;
	if (DEBUG)
		std::cerr << "Channel mode : " << rplParam << std::endl;
	std::string reply = director.rplChannelModeIs(this->_targetChannel->getName(), rplParam, this->_targetChannel->getPassword());
	this->_server->sendData(this->getClientFd(), reply);

}

void ModeCommand::execute() const
{

	std::string		possibleOperations = "+-";
	std::string		possibleModes = "itkol";
	unsigned int	operationNumber = possibleModes.find(this->_modeChar);
	std::string		reply;

	if (this->_params.size() == 1)
	{
		this->replyChannelMode();
		return ;
	}

	std::cerr << "mode char : " << this->_modeChar << std::endl;
	std::cerr << "operation char : " << this->_operationChar << std::endl;

	if (!this->_targetChannel->isOnChan(this->getClientFd()))
	{
		reply = _director.errNotOnChannel(this->_targetChannel->getName());
		this->_server->sendData(this->getClientFd(), reply);
		return ;
	}

	if (possibleModes.find(_modeChar) == possibleModes.npos)
	{
		reply = this->_director.errUnknownMode(_operationChar);
		this->_server->sendData(this->getClientFd(), reply);
		return;
	}

	if (this->_modeChar == "o")
	{
		std::string clientNick = this->_params.back().front();
		std::cerr << "trying to op " << clientNick << std::endl;
		Client	*targetClient = this->_server->getClient(clientNick);
		int	target = -1;
		if (!this->_targetChannel->isOp(this->getClientFd()))
		{
			reply = this->_director.errChanOPrivsNeeded(this->_targetChannel->getName());
			this->_server->sendData(this->getClientFd(), reply);
			return ;
		}
		if (targetClient == NULL || !(this->_targetChannel->isOnChan(target = this->_server->getClient(clientNick)->getFd())))
		{
			reply = this->_director.errNoSuchNick(clientNick);
			this->_server->sendData(this->getClientFd(), reply);
			return ;
		}
		if (this->_operationChar == "+")
			this->_targetChannel->setOperator(target);
		if (this->_operationChar == "-")
			this->_targetChannel->removeOperator(target);
	}
	this->_targetChannel->setModeItem(operationNumber, _operationChar[0] == '+');
	this->replyChannelMode();
	
}
