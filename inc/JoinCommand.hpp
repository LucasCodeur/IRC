/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbarru <kbarru@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 18:46:48 by kbarru            #+#    #+#             */
/*   Updated: 2026/05/07 13:44:08 by kbarru           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef JOIN_COMMAND_HPP
# define JOIN_COMMAND_HPP

# include "Command.hpp"
# include "Server.hpp"

class JoinCommand : public Command
{
public:
	JoinCommand(const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > params);
	~JoinCommand();

	void	execute(Server &server) const;

private:
	static const unsigned int min_params = 1;
	static const unsigned int max_params = 2;
};

#endif
