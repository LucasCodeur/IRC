#include "ModeCommand.hpp"
#include "Command.hpp"
#include "debug.hpp"
#include <iostream>

ModeCommand::ModeCommand(Server *server, const int clientFd, Command::t_msgSpecs specs, const std::vector<std::vector<std::string> > params) : Command(server, clientFd, specs, params)
{
	if (!server->getClient(clientFd)->isfullyRegistered())
	{
		throw Command::NotRegisteredException(server->getClientNickname(clientFd) + ":You are not registered");
	}
	std::string	modeEdition = "";
	std::string	reply;

	if (params.front().size() < ModeCommand::min_params)
		throw Command::NotEnoughParametersException(this->_command);

	std::string	targetChannelName = params.front().front();

	if (params.size() > 1)
		modeEdition = params[1].front(); // eg. +i

	if (params.size() == 2 && (modeEdition.length() != 2 || (modeEdition[0] != '+' && modeEdition[0] != '-')))
		throw Command::IncorrectParametersException(); //NOTE: maybe just silently ignore?

	if (DEBUG)
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

		
	if (((this->_operationChar == "+" && this->_modeChar == "k") || this->_modeChar == "o") && this->_params.size() < 3)
		throw Command::NotEnoughParametersException(this->_command);

	this->_targetChannel = this->_server->getChannelByName(targetChannelName);
	if (this->_targetChannel == NULL)
		throw Command::IncorrectParametersException("MODE : No such channel");


	// print every parameter for debug
	if (DEBUG)
	{
		std::cerr << "Mode command parameters :" << std::endl;
		for (size_t i = 0; i < params.size(); i++)
		{
			std::cerr << "Parameter " << i << " : ";
			for (size_t j = 0; j < params[i].size(); j++)
			{
				std::cerr << params[i][j] << " ";
			}
			std::cerr << std::endl;
		}
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
	std::string reply = director.rplChannelModeIs(this->getClient()->getNickname(), this->_targetChannel->getName(), rplParam, this->_targetChannel->getPassword());
	this->_server->writeInBuffer(this->getClient(), reply);

}

void	ModeCommand::changeUserMode() const
{
	std::string	reply;
	std::string clientNick = this->_params.back().front();
	Channel	*targetChannel = this->_targetChannel;

	if (DEBUG)
		std::cerr << "trying to add/remove " << clientNick << " op status on channel " << this->_targetChannel << std::endl;
	Client	*targetClient = this->_server->getClient(clientNick);
	int	target = this->_server->getClient(clientNick)->getFd();
	if (!this->_targetChannel->isOp(this->getClientFd()))
	{
		reply = this->_director.errChanOPrivsNeeded(clientNick, this->_targetChannel->getName());
		this->_server->writeInBuffer(this->getClient(), reply);
		return ;
	}

	if (targetClient == NULL || !(this->_targetChannel->isOnChan(target)))
	{
		reply = this->_director.errNoSuchNick(clientNick, this->_targetChannel->getName());
		this->_server->writeInBuffer(this->getClient(), reply);
		return ;
	}
	if (this->_operationChar == "+")
	{
		targetChannel->sendMessageToAll(":" + this->getClient()->getNickname() + " MODE " + targetChannel->getName() + " +o " + clientNick + "\r\n");
		this->_targetChannel->setOperator(target);
	}
	if (this->_operationChar == "-")
	{
		targetChannel->sendMessageToAll(":" + this->getClient()->getNickname() + " MODE " + targetChannel->getName() + " -o " + clientNick + "\r\n");
		this->_targetChannel->removeOperator(target);
	}
}

void ModeCommand::execute() const
{

	std::string		possibleOperations = "+-";
	std::string		possibleChanModes = "lkti";
	unsigned int	operationNumber = possibleChanModes.find(this->_modeChar);
	std::string		reply;

	if (this->_params.size() == 1)
	{
		this->replyChannelMode();
		return ;
	}

	if (DEBUG)
	{
		std::cerr << "mode char : " << this->_modeChar << std::endl;
		std::cerr << "operation char : " << this->_operationChar << std::endl;
	}

	if (!this->_targetChannel->isOp(this->getClientFd()))
	{
		reply = _director.errChanOPrivsNeeded(this->getClient()->getNickname(), this->_targetChannel->getName());
		this->_server->writeInBuffer(this->getClient(), reply);
		return ;
	}

	if (!this->_targetChannel->isOnChan(this->getClientFd()))
	{
		reply = _director.errNotOnChannel(this->getClient()->getNickname(), this->_targetChannel->getName());
		this->_server->writeInBuffer(this->getClient(), reply);
		return ;
	}

	if (possibleChanModes.find(_modeChar) == possibleChanModes.npos && _modeChar != "o")
	{
		reply = this->_director.errUnknownMode(this->getClient()->getNickname(), _modeChar);
		this->_server->writeInBuffer(this->getClient(), reply);
		return;
	}

	if (this->_modeChar == "o")
		this->changeUserMode();

	this->_targetChannel->setModeItem(operationNumber, _operationChar[0] == '+');
	this->replyChannelMode();
}
