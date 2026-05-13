#include "CommandFactory.hpp"
#include "Server.hpp"

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
	Command *topic1 = CommandFactory::createCommand(3, "TOPIC #publicchan");
	Command *topic2 = CommandFactory::createCommand(3, "TOPIC #publicchan :NEW TOPIC!!");
	Command *topic3 = CommandFactory::createCommand(3, "TOPIC #publicchan");

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

	topic1->execute(test);
	topic2->execute(test);
	topic3->execute(test);

	std::vector<int> protected_chan_users = test.getChannelMap().find("#protectedchan")->second->getUsers();

	// test.launcherServer();

	return (0);
}
