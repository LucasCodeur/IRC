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
#include "Exceptions.hpp"
#include "debug.hpp"

NickCommand::NickCommand(Server *server, const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > params) : Command(server, clientFd, type, params)
{
	size_t sizeParams = params.size();
	if (sizeParams < NickCommand::min_params)
		throw Command::IncorrectParametersException("Not enough parameters");
	else if (sizeParams > NickCommand::max_params)
		throw Command::IncorrectParametersException("Too much parameters");
	if (type != NICK)
		throw UnknownCommandException(); //FIXME: use appropriate exception for this
}

NickCommand::~NickCommand() {};

static bool	checkNickname(std::map<int, Client*>& map,  std::string nickname)
{
	for (std::map<int, Client*>::const_iterator it = map.begin(); it != map.end(); ++it)
	{
		if (nickname == it->second->getNickname())
			return (false);
	}
	return (true);
}

void	NickCommand::execute() const
{
	std::map<int, Client*> map = _server->getClientmap();
	std::map<int, Client*>::const_iterator it = map.find(this->getClientFd());
	std::string nickname = this->_params[0][0];
	
	if (checkNickname(map, nickname) == false)
	{
		PRINT("replicated nickname", BLUE, "\n");
		return ;
	}
	it->second->setNickname(nickname);
	it->second->authState.setNickReceived(true);
	// std::string message = "Nick information complete successfully\n";
	// if (send(this->getClientFd(), message.c_str(), message.size(), 0) < 0)
	// 	throw sendFailed();
}
