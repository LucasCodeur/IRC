#include "CommandFactory.hpp"
#include "Server.hpp"
#include "CommandFactory.hpp"
#include "Command.hpp"
#include <iostream>

void launchCmd(unsigned int fd, const std::string& cmd, Server& server)
{
	Command* command = CommandFactory::createCommand(&server, fd, cmd);
	command->execute();
	delete command;
}

int	main(int argc, char* argv[])
{
	(void)argv;
	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
		return 2;
	}
	Server test;

	test.launcherServer(argv[1], argv[2]);

	return (0);
}

// static void	testJoinCommand() {
// 	Server test;
// 	Command *join1 = CommandFactory::createCommand(&test, 1, "JOIN #protectedchan password");
// 	Command *join2 = CommandFactory::createCommand(&test, 2, "JOIN #protectedchan incorrect");
// 	Command *join3 = CommandFactory::createCommand(&test, 3, "JOIN #protectedchan");
// 	Command *join4 = CommandFactory::createCommand(&test, 4, "JOIN #publicchan");
// 	Command *join5 = CommandFactory::createCommand(&test, 5, "JOIN #publicchan");
// 	Command *join6 = CommandFactory::createCommand(&test, 6, "JOIN #publicchan unnecessaryPassword");
// 	Command *join7 = CommandFactory::createCommand(&test, 7, "JOIN #protectedchan password");
//
// 	try
// 	{
// 		Command *join8 = CommandFactory::createCommand(7, "JOIN #protectedchan password, password2"); // should throw
// 		join8->execute();
// 	}
// 	catch (std::exception &e)
// 	{
// 		std::cout << "caught : " << e.what() << std::endl;
// 	}
//
// 	// NOTE: memory gestion : should commands be deleted on execution?
// 	// NOTE: should the factory allocate the commands into a container? cf. resource ownership
//
// 	join1->execute();
// 	delete join1;
//
// 	join2->execute();
// 	delete join2;
//
// 	join3->execute();
// 	delete join3;
//
// 	join4->execute();
// 	delete join4;
//
// 	join5->execute();
// 	delete join5;
//
// 	join6->execute();
// 	delete join6;
//
// 	join7->execute();
// 	delete join7;
//
//
// 	std::vector<int> protected_chan_users = test.getChannelMap().find("#protectedchan")->second->getUsers();
//
// 	std::cout << "printing client list in #protected chan (should be 1, 7)" << std::endl;
// 	for(std::vector<int>::iterator it = protected_chan_users.begin(); it != protected_chan_users.end(); ++it)
// 		std::cout << *it << ";";
// 	std::cout << std::endl;
//
// }
