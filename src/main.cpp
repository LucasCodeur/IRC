/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enchevri <enchevri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 11:38:11 by lud-adam          #+#    #+#             */
/*   Updated: 2026/05/05 16:33:05 by enchevri         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

// #include "debug.hpp"
// #include "Client.hpp"
#include "Command.hpp"
#include "Server.hpp"
// #include "Channel.hpp"
#include <cstdlib>
// #include <iostream>
#include <vector>

int	main(int ac, char** av)
{
	if (ac != 3)
		return 2;
	Server server(std::atoi(av[1]), av[2]);
	std::vector<std::string> param;
	param.push_back("#Test");
	param.push_back("#Test2");
	Command cmd1(1, Command::JOIN, param);	
	Command cmd2(1, Command::JOIN, param);	
	cmd1.setClientFd(1);
	cmd2.setClientFd(1);
	server.handleCommand(cmd1);
	server.handleCommand(cmd2);	
	return (0);
}
