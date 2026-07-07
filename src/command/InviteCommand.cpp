#include "InviteCommand.hpp"

InviteCommand::InviteCommand(Server *server, const int clientFd, t_msgSpecs specs, const std::vector<std::vector<std::string> > params) : Command(server, clientFd, specs, params)
{
	if (!server->getClient(clientFd)->isfullyRegistered())
	{
		throw Command::NotRegisteredException(server->getClientNickname(clientFd) + ":You are not registered");
	}
	if (params.size() < InviteCommand::min_params)
		throw Command::NotEnoughParametersException("Not enough parameters");
	else if (params.size() > InviteCommand::max_params)
		throw Command::NotEnoughParametersException("Too much parameters");
	
	this->_invitedNick = this->_params.front()[0];
	this->_targetChannel = this->_params.back()[0];
}

InviteCommand::~InviteCommand() {}

void InviteCommand::execute() const
{
	Channel *distTargetChan = this->_server->getChannelByName(this->_targetChannel);
	Client	*distTargetClient = this->_server->getClient(this->_invitedNick);

	std::string reply;

	if (distTargetChan == NULL)
	{
		reply = this->_director.errNoSuchChannel(this->getClient()->getNickname(), this->_targetChannel);
		this->_server->writeInBuffer(this->getClient(), reply);
		return ;
	}

	if (distTargetClient == NULL)
	{
		reply = this->_director.errNoSuchNick(this->getClient()->getNickname(), this->_targetChannel);
		this->_server->writeInBuffer(this->getClient(), reply);
		return ;
	}

	if (distTargetChan->isOnChan(this->_invitedNick))
	{
		reply = this->_director.errUserOnChannel(this->getClient()->getNickname(), this->_invitedNick, this->_targetChannel);
		this->_server->writeInBuffer(this->getClient(), reply);
		return ;
	}

	if (!distTargetChan->isOnChan(this->getClient()->getNickname()))
	{
		reply = this->_director.errNotOnChannel(this->getClient()->getNickname(), this->_targetChannel);
		this->_server->writeInBuffer(this->getClient(), reply);
		return ;
	}

	if (!distTargetChan->isOp(this->getClient()->getFd()))
	{
		reply = this->_director.errChanOPrivsNeeded(this->getClient()->getNickname(), this->_targetChannel);
		this->_server->writeInBuffer(this->getClient(), reply);
		return ;
	}

	distTargetChan->addInvite(distTargetClient->getFd());
	reply = this->_director.rplInviting(this->getClient()->getNickname(), this->_invitedNick, this->_targetChannel);
	this->_server->writeInBuffer(this->getClient(), reply);
}
