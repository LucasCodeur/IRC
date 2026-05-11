/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lud-adam <lud-adam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 15:53:21 by lud-adam          #+#    #+#             */
/*   Updated: 2026/05/11 17:28:03 by lud-adam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <utility>
#include <iostream>
#include "Command.hpp"
#include "debug.hpp"
#include "UserCommand.hpp"

UserCommand::UserCommand(const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > params) : Command(clientFd, type, params)
{
	if (params.size() < UserCommand::min_params)
		throw Command::IncorrectParametersException("Not enough parameters");
	else if (params.size() > UserCommand::max_params)
		throw Command::IncorrectParametersException("Too much parameters");
	if (type != USER)
		throw UnknownCommandException(); //FIXME: use appropriate exception for this
	
	std::vector<std::string> channels = params.front();
	std::vector<std::string> keys;
	if (params.size() == UserCommand::max_params)
		keys = params.back();
	else //NOTE: no keys provided; creating dummy keys vector so execute() has something to parse
		this->_params.push_back(std::vector<std::string>());
	if (channels.size() < keys.size())
		throw Command::IncorrectParametersException("More keys than channels");
}
