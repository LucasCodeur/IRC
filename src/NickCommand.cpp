/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lud-adam <lud-adam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 14:58:58 by lud-adam          #+#    #+#             */
/*   Updated: 2026/05/28 14:59:04 by lud-adam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "NickCommand.hpp"
#include "Client.hpp"
#include "debug.hpp"
#include "Exceptions.hpp"
#include <cctype>

NickCommand::NickCommand(Server *server, const int clientFd, t_msgSpecs specs, const std::vector<std::vector<std::string> > params) : Command(server, clientFd, specs, params)
{
	size_t sizeParams = params.size();
	std::string reply;
	if (sizeParams < NickCommand::min_params)
	{
		reply = this->getDirector()->errNonicknamegiven();
		if (send(this->getClientFd(), reply.c_str(), reply.size(), 0) < 0)
			throw sendFailed();
		throw Command::IncorrectParametersException("Not enough parameters");
	}
	else if (sizeParams > NickCommand::max_params)
		throw Command::IncorrectParametersException("Too much parameters");
}

NickCommand::~NickCommand() {};

static bool	check_nickname(std::string& nickname);
static bool	checkCollisionNickname(std::map<int, Client*>& map,  std::string nickname);

void	NickCommand::execute() const
{
	std::map<int, Client*> map = _server->getClientmap();
	std::map<int, Client*>::const_iterator it = map.find(this->getClientFd());
	Client* client = it->second;
	std::string nickname = this->_params[0][0];
	
	if (checkCollisionNickname(map, nickname) == false && client->authState.getFullyRegistered() == true)
	{
		std::string reply;
		reply = this->_director.errNicknameinuse(nickname);
		if (send(this->getClientFd(), reply.c_str(), reply.size(), 0) < 0)
			throw sendFailed();
		return ;
	}
	else if (checkCollisionNickname(map, nickname) == false)
	{
		std::string reply;
		reply = this->_director.errNickcollision(nickname);
		if (send(this->getClientFd(), reply.c_str(), reply.size(), 0) < 0)
			throw sendFailed();
		return ;
	}
	else if (check_nickname(nickname) == false)
	{
		std::string reply;
		reply = this->_director.errErroneusnickname(nickname);
		if (send(this->getClientFd(), reply.c_str(), reply.size(), 0) < 0)
			throw sendFailed();
		return ;
	}
	client->setNickname(nickname);
	client->authState.setNickReceived(true);
}

static bool	checkCollisionNickname(std::map<int, Client*>& map,  std::string nickname)
{
	for (std::map<int, Client*>::const_iterator it = map.begin(); it != map.end(); ++it)
	{
		if (nickname == it->second->getNickname())
			return (false);
	}
	return (true);
}

static bool	special_charset(char c);

static bool	check_nickname(std::string& nickname)
{
	if (std::isalpha(nickname[0]) == false)
		return (false);
	for (int i = 1; nickname[i]; i++)
	{
		if (std::isalpha(nickname[i]) == false && std::isdigit(nickname[i]) == false && special_charset(nickname[i]) == false)
			return (false);
	}
	return (true);
}

static bool	special_charset(char c)
{
	if (c == '-' || c == '[' || c ==']' || c == '\\' || c =='`' || c== '^' || c == '{' || c == '}')
		return (true);
	return (false);
}
