#include "ModeCommand.hpp"
#include "Command.hpp"
#include "NumericReplies.h"
#include <iostream>

ModeCommand::ModeCommand(Server *server, const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > params) : Command(server, clientFd, type, params)
{
	std::string	targetChannel = params.front().front();
	std::string	modeEdition = params.front()[1];

	this->operationChar = params.front()[1][0];

	if (params.size() < ModeCommand::min_params)
		this->returnErrorReply(ERR_NEEDMOREPARAMS, "MODE", *_server);

	else if (modeEdition.length() < 2 || modeEdition.length() > 2)
		throw Command::IncorrectParametersException(); //TODO: same
	

}

ModeCommand::~ModeCommand() {}

void ModeCommand::execute() const
{

	std::string		possibleOperations = "+-";
	std::string		possibleModes = "itkol";
	unsigned int	operation;

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
}
