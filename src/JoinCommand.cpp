#include <utility>
#include <iostream>
#include <sstream>
#include "Channel.hpp"
#include "Client.hpp"
#include "ReplyBuilder.hpp"
#include "Command.hpp"
#include "debug.hpp"
#include "JoinCommand.hpp"
#include "NumericReplies.h"

JoinCommand::JoinCommand(Server *server, const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > params) : Command(server, clientFd, type, params)
{
	if (params.size() < JoinCommand::min_params)
		throw Command::IncorrectParametersException("Not enough parameters");
	else if (params.size() > JoinCommand::max_params)
		throw Command::IncorrectParametersException("Too much parameters");
	if (type != JOIN)
		throw UnknownCommandException(); //FIXME: use appropriate exception for this
	
	std::vector<std::string> channels = params.front();
	std::vector<std::string> keys;
	if (params.size() == JoinCommand::max_params)
		keys = params.back();
	else //NOTE: no keys provided; creating dummy keys vector so execute() has something to parse
		this->_params.push_back(std::vector<std::string>());
	if (channels.size() < keys.size())
		throw Command::IncorrectParametersException("More keys than channels");
}

JoinCommand::~JoinCommand() {}

void JoinCommand::sendReply(Client const &client, Channel const &channel) const
{
	Director director;

	channel.sendMessageToAll(director.rplJoin(client, channel).c_str());

	if (channel.getTopic().empty()) //send topic or no topic reply
		this->_server->sendData(client.getFd(), director.rplNoTopic(client, channel));
	else
		this->_server->sendData(client.getFd(), director.rplTopic(client, channel));

	std::string namesList;
	std::vector<int> const &users = channel.getUsers();
	for (size_t i = 0; i < users.size(); i++)
	{
		if (i > 0) namesList += " ";
		if (channel.isOp(users[i]))
			namesList += "@";
		namesList += this->_server->getClientNickname(users[i]); 
	}

	this->_server->sendData(client.getFd(), director.rplNameReply(client, channel, namesList)); //send names list
	this->_server->sendData(client.getFd(), director.rplEndOfNames(client, channel));//send end of names list
}

void JoinCommand::sendErrorReply(Client const &client, Channel const &channel, std::string const &numeric) const
{
	Director director;

	std::string reply = director.rplError(numeric, client, channel);
	this->_server->sendData(client.getFd(), reply);
}

/**
* @brief executes itself.
* @param server the server in which the JoinCommand should be executed.
* A JoinCommand is in the form JOIN channel_1, channel_2 ... pass1, pass2 ...
* the function iterates over each channel_i and verifies wether :
	* - the channel already exists (if not, create it)
	* - the provided pass_i is correct.
* if everything is good for a given channel, call server::addUser to add the current user to said channel.
* then proceed for next provided channel.
**/
// NOTE: separate password verification logic? Maybe in Server class?
// NOTE: separate channel creation ?
// TODO: replace raw channel.addUser with dedicated USER command (could check if user is already in there etc.)
void JoinCommand::execute() const
{
	std::vector<std::string> keys = this->_params.back();
	std::vector<std::string> channels = this->_params.front();

	std::map<std::string, Channel *> const &channelMap = this->_server->getChannelMap();

	std::vector<std::string>::const_iterator key_it = keys.begin();
	std::vector<std::string>::iterator chan_it;

	std::map<int, Client*>::const_iterator it = this->_server->getClientmap().find(this->getClientFd());

	std::string providedPassword = "";

	for (chan_it = channels.begin(); chan_it != channels.end(); ++chan_it)
	{
		std::map<std::string, Channel *>::const_iterator distChan_it = channelMap.find(*chan_it);
		if (distChan_it == channelMap.end()) // channel creation
		{
			std::pair<std::map<std::string, Channel *>::iterator, bool> pair;

			if (key_it != keys.end())
				pair = this->_server->addChannel(*chan_it, *key_it);
			else
				pair = this->_server->addChannel(*chan_it, "");
			distChan_it = channelMap.find(*chan_it);
			if (distChan_it == channelMap.end()) // FIXME: should probably throw there
			{
				std::cerr << "Error : channel could not be created" << std::endl;
				return ;
			}
			distChan_it->second->addUser(this->getClientFd());
			distChan_it->second->setOperator(this->getClientFd());

			this->sendReply(*(it->second), *(distChan_it->second));
			
			if (key_it != keys.end())
				key_it++;
			continue ;
		}

		providedPassword = "";
		if (key_it != keys.end())
			providedPassword = *key_it++;

		if (distChan_it->second->getPassword() == "") // if no password required
		{
			distChan_it->second->addUser(this->getClientFd());
			{
				this->sendReply(*(it->second), *(distChan_it->second));
			}
		}
		else if (distChan_it->second->getPassword() == providedPassword) // if password correct
		{
			distChan_it->second->addUser(this->getClientFd());
			{
				this->sendReply(*(it->second), *(distChan_it->second));
			}
		}
		else // if password incorrect
		{
			this->sendErrorReply(*(it->second), *(distChan_it->second), ERR_BADCHANNELKEY);
			continue ;
		}
	}
}
