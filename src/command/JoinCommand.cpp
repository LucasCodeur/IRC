#include <ostream>
#include <utility>
#include <iostream>
#include "Channel.hpp"
#include "Client.hpp"
#include "ReplyBuilder.hpp"
#include "Command.hpp"
#include "JoinCommand.hpp"

static void tolower(std::string &str)
{
	for (std::string::iterator it = str.begin(); it != str.end(); ++it)
		*it = std::tolower(*it);
}

JoinCommand::JoinCommand(Server *server, const int clientFd, t_msgSpecs specs, const std::vector<std::vector<std::string> > params) : Command(server, clientFd, specs, params)
{
	if (!server->getClient(clientFd)->isfullyRegistered())
	{
		throw Command::NotRegisteredException(server->getClientNickname(clientFd) + ":You are not registered");
	}

	if (params.size() < JoinCommand::min_params)
		throw Command::NotEnoughParametersException("Not enough parameters");
	else if (params.size() > JoinCommand::max_params)
		throw Command::NotEnoughParametersException("Too much parameters");
	
	std::vector<std::string> channels = params.front();
	std::vector<std::string> keys;
	if (params.size() == JoinCommand::max_params)
		keys = params.back();
	else //NOTE: no keys provided; creating dummy keys vector so execute() has something to parse
		this->_params.push_back(std::vector<std::string>());
	if (channels.size() < keys.size())
		throw Command::NotEnoughParametersException("More keys than channels");
}

JoinCommand::~JoinCommand() {}

void JoinCommand::confirmJoin(Client &client, Channel const &channel) const
{
	channel.sendMessageToAll(this->_server, this->_director.rplJoin(client, channel).c_str());

	if (channel.getTopic().empty())
		this->_server->writeInBuffer(&client, this->_director.rplNoTopic(client, channel));
	else
		this->_server->writeInBuffer(&client, this->_director.rplTopic(client, channel));

	this->_server->writeInBuffer(&client, this->_director.rplNameReply(client, channel, channel.getChannelNamesList()));
	this->_server->writeInBuffer(&client, this->_director.rplEndOfNames(client, channel));
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

	int clientFd = this->getClientFd(); 
	std::map<int, Client*>::const_iterator it = this->_server->getClientmap().find(clientFd);
	
	std::string providedPassword = "";
	for (chan_it = channels.begin(); chan_it != channels.end(); ++chan_it)
	{
		tolower(*chan_it);
		std::map<std::string, Channel *>::const_iterator distChan_it = channelMap.find(*chan_it);
		if (distChan_it == channelMap.end()) // channel creation
		{
			std::pair<std::map<std::string, Channel *>::iterator, bool> pair;

			if (key_it != keys.end())
				pair = this->_server->addChannel(*chan_it, *key_it);
			else
				pair = this->_server->addChannel(*chan_it, "");

			distChan_it = channelMap.find(*chan_it);
			if (distChan_it == channelMap.end())
			{
				std::cerr << "Error : channel '" << *chan_it << "' could not be created" << std::endl;
				this->_server->writeInBuffer(this->getClient(),
					this->_director.errNoSuchChannel(this->getClient()->getNickname(), *chan_it));
				if (key_it != keys.end())
					key_it++;
				continue ;
			}
			distChan_it->second->addUser(this->_server->getClient(clientFd));
			distChan_it->second->setOperator(clientFd);
			this->confirmJoin(*(it->second), *(distChan_it->second));
			if (key_it != keys.end())
				key_it++;
			continue;
		}

		providedPassword = "";
		if (distChan_it->second->isUserInChannel(clientFd)) // if already in channel, ignore
		{
			if (key_it != keys.end())
				key_it++;
			continue;
		}
		if (key_it != keys.end())
			providedPassword = *key_it++;

		if (distChan_it->second->isInviteOnly() && !distChan_it->second->isInvited(clientFd))
		{
			std::string reply = this->_director.errInviteOnlyChan(this->getClient()->getNickname(), *chan_it);
			this->_server->writeInBuffer(this->getClient(), reply);
			continue;
		}

		if (distChan_it->second->hasUserLimit() && distChan_it->second->getUsers().size() >= distChan_it->second->getUserLimit())
		{
			std::string reply = this->_director.errChannelIsFull(this->getClient()->getNickname(), *chan_it);
			this->_server->writeInBuffer(this->getClient(), reply);
			continue;
		}

		if (distChan_it->second->getPassword() == "") // if no password required
		{
			distChan_it->second->addUser(this->_server->getClient(clientFd));
			distChan_it->second->removeInvite(clientFd);
			this->confirmJoin(*(it->second), *(distChan_it->second));
		}
		else if (distChan_it->second->getPassword() == providedPassword) // if password correct
		{
			distChan_it->second->addUser(this->_server->getClient(clientFd));
			distChan_it->second->removeInvite(clientFd);
			this->confirmJoin(*(it->second), *(distChan_it->second));
		}
		else // if password incorrect
		{
			std::string reply = this->_director.errBadChannelKey(this->getClient()->getNickname(), *chan_it);
			this->_server->writeInBuffer(this->getClient(), reply);
		}
	}
}
