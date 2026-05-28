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
