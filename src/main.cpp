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
	{
		std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
		return 2;
	}
	Server test;
	try
	{
		launchCmd(1, "JOIN #test,#lolix", test);
		launchCmd(3, "JOIN #test,#lolix", test);
		launchCmd(2, "JOIN #lol", test);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}
	
	return (0);
}
