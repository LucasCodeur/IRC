/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lud-adam <lud-adam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 15:52:55 by lud-adam          #+#    #+#             */
/*   Updated: 2026/05/20 11:46:41 by lud-adam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_COMMAND_HPP
# define USER_COMMAND_HPP

# include "Command.hpp"
# include "Server.hpp"

class UserCommand : public Command
{
	public:
		UserCommand(const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > params);
		~UserCommand();

		void	execute(Server &server) const;

	private:
		static const unsigned int min_params = 4;
		static const unsigned int max_params = 4;
};

#endif
