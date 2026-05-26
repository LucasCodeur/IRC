#include "ModeCommand.hpp"
#include "Command.hpp"
#include "NumericReplies.h"
#include <iostream>
#include <sstream>

ModeCommand::ModeCommand(Server *server, const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > params) : Command(server, clientFd, type, params)
{
	std::string	targetChannel = params.front().front();
	std::string	modeEdition = params.front()[1];

	this->operationChar = params.front()[1][0];

	if (params.size() < ModeCommand::min_params)
		this->returnErrorReply(ERR_NEEDMOREPARAMS, "MODE", *_server);

	else if (modeEdition.length() < 2 || modeEdition.length() > 2)
		throw Command::IncorrectParametersException(); //NOTE: maybe just silently ignore?
	
	Channel * _targetChannel = this->_server->getChannelByName(targetChannel);
	if (_targetChannel == NULL)
		this->returnErrorReply(ERR_NOSUCHCHANNEL, targetChannel, *_server);
	else
		this->_targetChannel = _targetChannel;

}

ModeCommand::~ModeCommand() {}

void ModeCommand::replyChannelMode() const
{
	std::stringstream    rplParam;
	if (this->_targetChannel->isInviteOnly())
		rplParam << "i";
	if (this->_targetChannel->isTopicRestricted())
		rplParam << "t";
	if (this->_targetChannel->hasPassword())
		rplParam << "k";
	if (this->_targetChannel->hasUserLimit())
		rplParam << "l";
	this->returnErrorReply(RPL_CHANNELMODEIS, rplParam.str(), *this->_server);
}

void ModeCommand::execute() const
{

	std::string		possibleOperations = "+-";
	std::string		possibleModes = "itkol";
	unsigned int	operation;

	if (this->_params.size() == 1)
	{
		this->replyChannelMode();
		return ;
	}

	for (size_t	i = 0; i < possibleModes.length(); ++i)
	{
		operation = i;
		if (operationChar[0] == possibleOperations[i])
			break;
	}

	std::cerr << "Current operation : " << operation << std::endl;

	if (operation == UNKNOWNMODE)
	{
		this->returnErrorReply(ERR_UNKNOWNMODE, this->operationChar, *_server);
		return ;
	}

	if (possibleModes.find(operationChar) == possibleModes.npos)
	{
		this->returnErrorReply(ERR_UNKNOWNMODE , operationChar, *_server);
		return;
	}

	std::cerr << "Adding mode " << possibleModes[operation] << std::endl;
	this->_targetChannel->setModeItem(operation, operationChar[0] == '+');

	this->replyChannelMode();
	
}
