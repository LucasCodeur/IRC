/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lud-adam <lud-adam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 19:10:38 by lud-adam          #+#    #+#             */
/*   Updated: 2026/05/12 19:19:30 by lud-adam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PASS_COMMAND_HPP
# define PASS_COMMAND_HPP

# include "Command.hpp"
# include "Server.hpp"

class PassCommand : public Command
{
	public:
		PassCommand(const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > params, std::string server_password);
		~PassCommand();

		void	execute(Server &server) const;

	private:
		static const unsigned int min_params = 1;
		static const unsigned int max_params = 1;
};

#endif
