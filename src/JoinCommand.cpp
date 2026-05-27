#include <utility>
#include <iostream>
#include <sstream>
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
	Director director;
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
			std::string reply = director.rplJoin(*(it->second), *chan_it);
			distChan_it->second->sendMessageToAll(reply.c_str());
			if (key_it != keys.end())
				key_it++;
			continue ;
		}

		providedPassword = "";
		if (key_it != keys.end())
			providedPassword = *key_it++;

		if (distChan_it->second->getPassword() == "") // if no password required
		{
			if (DEBUG)
				std::cout << DBUG << this->getClientFd() << GREEN " joining " << *chan_it << " with no pass required" RESET << std::endl;
			distChan_it->second->addUser(this->getClientFd());
			{
				std::string reply = director.rplJoin(*(it->second), *chan_it);
				distChan_it->second->sendMessageToAll(reply.c_str()); //TODO: change after the builder is done
			}
		}
		else if (distChan_it->second->getPassword() == providedPassword) // if password correct
		{
			if (DEBUG)
				std::cout << DBUG << this->getClientFd() << GREEN " joining " << *chan_it << " with correct pass '" << providedPassword << "'" RESET << std::endl;
			distChan_it->second->addUser(this->getClientFd());
			{
				std::stringstream joinMsg;
				joinMsg << ":" << this->_server->getClientNickname(this->getClientFd()) << " JOIN " << *chan_it << "\r\n";
				distChan_it->second->sendMessageToAll(joinMsg.str()); //TODO: change after the builder is done
			}
		}
		else // if password incorrect
		{
			std::stringstream ss;
			ss << this->_server->getClientNickname(this->getClientFd()) << " " << *chan_it;
			this->returnErrorReply(ERR_BADCHANNELKEY, ss.str(), *this->_server);//TODO: change after the builder is done
		}
	}
}
