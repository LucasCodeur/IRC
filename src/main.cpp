/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enchevri <enchevri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 11:38:11 by lud-adam          #+#    #+#             */
/*   Updated: 2026/04/27 17:08:06 by enchevri         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

// #include "debug.hpp"
#include "Client.hpp"
#include "Command.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include <iostream>
#include <vector>

int	main(void)
{
	Server server;
	std::vector<std::string> param;
	param.push_back("#Test");
	Command cmd1(1, Command::JOIN, param);	
	Command cmd2(2, Command::JOIN, param);	
	cmd1.setClientFd(1);
	cmd1.setClientFd(2);
	server.handleCommand(cmd1);
	server.handleCommand(cmd2);		
	return (0);
}
