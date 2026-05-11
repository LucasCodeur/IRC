/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbarru <kbarru@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 10:14:43 by kbarru            #+#    #+#             */
/*   Updated: 2026/05/11 11:59:33 by kbarru           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <utility>
#include <iostream>
#include "Command.hpp"
#include "debug.hpp"
#include "JoinCommand.hpp"

JoinCommand::JoinCommand(const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > params) : Command(clientFd, type, params)
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
void JoinCommand::execute(Server &server) const
{
	std::vector<std::string> keys = this->_params.back();
	std::vector<std::string> channels = this->_params.front();

	std::map<std::string, Channel *> channelMap = server.getChannelMap();

	std::vector<std::string>::iterator key_it = keys.begin();
	std::vector<std::string>::iterator chan_it;

	std::string providedPassword = "";

	for (chan_it = channels.begin(); chan_it != channels.end(); ++chan_it)
	{
		std::map<std::string, Channel *>::iterator distChan_it = channelMap.find(*chan_it);
		if (distChan_it == channelMap.end()) // channel creation
		{
			std::pair<std::map<std::string, Channel *>::iterator, bool> pair;

			if (key_it != keys.end())
				pair = server.addChannel(*chan_it, *key_it);
			else
				pair = server.addChannel(*chan_it, "");
		}

		std::map<std::string, Channel *> channelMap = server.getChannelMap();
		distChan_it = channelMap.find(*chan_it);

		if (distChan_it == channelMap.end()) // FIXME: should probably throw there
		{
			std::cerr << "Error : channel could not be created" << std::endl;
			return ;
		}

		providedPassword = "";
		if (key_it != keys.end())
			providedPassword = *key_it++;

		if (distChan_it->second->getPassword() == providedPassword) // if password correct
		{
			std::cout << DBUG << this->getClientFd() << GREEN " joining " << *chan_it << " with correct pass '" << providedPassword << "'" RESET << std::endl;
			distChan_it->second->addUser(this->getClientFd());
		}
		else // provided password incorrect OR password provided for non-password channel
			std::cout << DBUG << this->getClientFd() << RED " could NOT join : " << *chan_it << " auth failure '" << providedPassword << "'" << RESET << std::endl;
	}
}
