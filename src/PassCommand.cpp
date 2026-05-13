/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lud-adam <lud-adam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 19:10:15 by lud-adam          #+#    #+#             */
/*   Updated: 2026/05/13 10:23:07 by lud-adam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <utility>
#include <iostream>
#include "Command.hpp"
#include "debug.hpp"
#include "PassCommand.hpp"
#include "Exceptions.hpp"

PassCommand::PassCommand(const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > params, std::string server_password) : Command(clientFd, type, params)
{
	if (params.size() < PassCommand::min_params)
		throw Command::IncorrectParametersException("Not enough parameters");
	else if (params.size() > PassCommand::max_params)
		throw Command::IncorrectParametersException("Too much parameters");
	if (type != PASS)
		throw UnknownCommandException(); //FIXME: use appropriate exception for this
	
	std::string password = params[0][0];
	if (password != server_password)
		throw passwordNotCorrect();
}
