/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TopicCommand.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbarru <kbarru@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 11:38:09 by kbarru            #+#    #+#             */
/*   Updated: 2026/05/20 15:21:41 by kbarru           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOPIC_COMMAND_HPP
# define TOPIC_COMMAND_HPP

#include "Server.hpp"
#include "Command.hpp"

class TopicCommand : public Command
{

public:
	TopicCommand(Server *server, const int clientFd, t_msgSpecs specs, const std::vector <std::vector<std::string> > params);
	void executeTopicChange(Channel *distTargetChannel) const;
	~TopicCommand();

	void	execute() const;

private:
	static const unsigned int min_params = 1;
	static const unsigned int max_params = 2;
	std::string	_targetChannel;
	std::string	_newTopic;

};

#endif
