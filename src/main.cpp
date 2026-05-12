#include "Server.hpp"

int	main(int argc, char* argv[])
{
	(void)argv;
	if (argc != 3)
		return 2;
	Server server;

	server.launcherServer();

	return (0);
}
