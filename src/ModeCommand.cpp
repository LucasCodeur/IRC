#include "ModeCommand.hpp"
#include "Command.hpp"
#include <iostream>
#include <sstream>

ModeCommand::ModeCommand(Server *server, const int clientFd, Command::t_msgSpecs specs, const std::vector<std::vector<std::string> > params) : Command(server, clientFd, specs, params)
{
	std::string	targetChannel = params.front().front();
	std::string	modeEdition = "";
	std::string	reply;

	if (modeEdition.length() > 1)
	{
		modeEdition = params.front()[1];
		this->operationChar = params.front()[1][0];
		std::cout << "Mode edition : " << modeEdition << std::endl;
	}
	else
		this->operationChar = '\0';


	if (params.size() < ModeCommand::min_params)
	{
		reply = this->_director.errNeedMoreParams(*_server->getClientByFd(this->getClientFd()), "MODE");
		this->_server->sendData(this->getClientFd(), reply);
	}
	else if (params.size() == 2 && (modeEdition.length() != 2 || (modeEdition[0] != '+' && modeEdition[0] != '-')))
		throw Command::IncorrectParametersException(); //NOTE: maybe just silently ignore?
	
	Channel * _targetChannel = this->_server->getChannelByName(targetChannel);
	if (_targetChannel == NULL)
	{
		reply = this->_director.errNoSuchChannel(*_server->getClientByFd(this->getClientFd()), targetChannel);
		throw std::runtime_error(reply);
		this->_server->sendData(this->getClientFd(), reply);
	}
	else
		this->_targetChannel = _targetChannel;
}

ModeCommand::~ModeCommand() {}

void ModeCommand::replyChannelMode() const
{
	Director director;
	std::stringstream    rplParam;
	if (this->_targetChannel->isInviteOnly())
		rplParam << "i";
	if (this->_targetChannel->isTopicRestricted())
		rplParam << "t";
	if (this->_targetChannel->hasPassword())
		rplParam << "k";
	if (this->_targetChannel->hasUserLimit())
		rplParam << "l";
	std::string reply = director.rplChannelModeIs(*this->_server->getClientByFd(this->getClientFd()), this->_targetChannel->getName(), rplParam.str());
	this->_server->sendData(this->getClientFd(), reply);

}

void ModeCommand::execute() const
{

	std::string		possibleOperations = "+-";
	std::string		possibleModes = "itkol";
	std::string		reply;
	unsigned int	operation;

	if (this->_params.size() == 1)
	{
		this->replyChannelMode();
		return ;
	}

	if (!this->_targetChannel->isOnChan(this->getClientFd()))
	{
		reply = _director.errNotOnChannel(*this->_server->getClientByFd(this->getClientFd()), this->_targetChannel->getName());
		this->_server->sendData(this->getClientFd(), reply);
		return ;
	}
	std::cerr << "Operation char : " << this->operationChar << std::endl;
	for (size_t	i = 0; i < possibleModes.length(); ++i)
	{
		operation = i;
		if (operationChar[0] == possibleOperations[i])
			break;
	}

	std::cerr << "Current operation : " << operation << std::endl;

	if (operation == UNKNOWNMODE) //NOTE: redundant check : already there in constructor
	{
		reply = this->_director.errUnknownMode(*_server->getClientByFd(this->getClientFd()), operationChar);
		this->_server->sendData(this->getClientFd(), reply);
		return ;
	}

	if (possibleModes.find(operationChar) == possibleModes.npos)
	{
		reply = this->_director.errUnknownMode(*_server->getClientByFd(this->getClientFd()), operationChar);
		this->_server->sendData(this->getClientFd(), reply);
		return;
	}

	std::cerr << "Adding mode " << possibleModes[operation] << std::endl;
	this->_targetChannel->setModeItem(operation, operationChar[0] == '+');

	this->replyChannelMode();
	
}
