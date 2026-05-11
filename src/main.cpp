/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enchevri <enchevri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 11:38:11 by lud-adam          #+#    #+#             */
/*   Updated: 2026/05/07 16:47:18 by kbarru           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "CommandFactory.hpp"
#include "Server.hpp"
#include <iostream>

int	main(int argc, char* argv[])
{
	(void)argv;
	if (argc != 3)
		return 2;
	Server test;

	Command *join1 = CommandFactory::createCommand(1, "JOIN #protectedchan password");
	Command *join2 = CommandFactory::createCommand(2, "JOIN #protectedchan incorrect");
	Command *join3 = CommandFactory::createCommand(3, "JOIN #protectedchan");
	Command *join4 = CommandFactory::createCommand(4, "JOIN #publicchan");
	Command *join5 = CommandFactory::createCommand(5, "JOIN #publicchan");
	Command *join6 = CommandFactory::createCommand(6, "JOIN #publicchan unnecessaryPassword");
	Command *join7 = CommandFactory::createCommand(7, "JOIN #protectedchan password");

	try
	{
		Command *join8 = CommandFactory::createCommand(7, "JOIN #protectedchan password, password2"); // should throw
		join8->execute(test);
	}
	catch (std::exception &e)
	{
		std::cout << "caught : " << e.what() << std::endl;
	}

	// NOTE: memory gestion : should commands be deleted on execution?
	// NOTE: should the factory allocate the commands into a container? cf. resource ownership

	join1->execute(test);
	delete join1;

	join2->execute(test);
	delete join2;

	join3->execute(test);
	delete join3;

	join4->execute(test);
	delete join4;

	join5->execute(test);
	delete join5;

	join6->execute(test);
	delete join6;
	
	join7->execute(test);
	delete join7;


	std::vector<int> protected_chan_users = test.getChannelMap().find("#protectedchan")->second->getUsers();

	std::cout << "printing client list in #protected chan (should be 1, 7)" << std::endl;
	for(std::vector<int>::iterator it = protected_chan_users.begin(); it != protected_chan_users.end(); ++it)
		std::cout << *it << ";";
	std::cout << std::endl;

	// test.launcherServer();

	return (0);
}
