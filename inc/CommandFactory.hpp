/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandFactory.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbarru <kbarru@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 16:28:00 by kbarru            #+#    #+#             */
/*   Updated: 2026/05/07 11:23:47 by kbarru           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_FACTORY_HPP
# define COMMAND_FACTORY_HPP

#include "Command.hpp"

class CommandFactory
{
public:
	CommandFactory();
	virtual ~CommandFactory() = 0;
	static Command *createCommand(const int clientFd, const std::string str);

	
private:
	// Command	*createTopicCommand(const int clientFd, const enum Command::commandType type, const std::vector<std::vector<<std::string> > params) const;
	static Command	*createJoinCommand(const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > params);
	// Command	*createKickCommand(const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > params) const;
	// Command	*createModeCommand(const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > params) const;
	// Command	*createWhoCommand(const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > params) const;
	// Command	*createPassCommand(const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > params) const;
	// Command	*createNickCommand(const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > params) const;
	// Command	*createUserCommand(const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > params) const;
	// Command	*createPartCommand(const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > params) const;
};

#endif
