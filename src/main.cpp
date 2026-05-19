#include "CommandFactory.hpp"
#include "Server.hpp"
#include "CommandFactory.hpp"
#include "Command.hpp"
#include <iostream>

void launchCmd(unsigned int fd, const std::string& cmd, Server& server)
{
	Command* command = CommandFactory::createCommand(fd, cmd);
	command->execute(server);
	delete command;
}

int	main(int argc, char* argv[])
{
	(void)argv;
	if (argc != 3)
		return 2;
	Server test;
	try
	{
		launchCmd(1, "JOIN #test", test);
		launchCmd(1, "PART #test", test);
		launchCmd(2, "JOIN #lol", test);
		launchCmd(2, "PART #test,#lol,#lolix", test);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}
	
	return (0);
}
