#include "Server.hpp"

int	main(int argc, char* argv[])
{
	(void)argv;
	if (argc != 3)
		return 2;
	Server test;
	
	test.launcherServer();
	return (0);
}
