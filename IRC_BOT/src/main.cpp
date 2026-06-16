#include "Bot.hpp"

#include <iostream>

#include <csignal>

void signalHandler(int signum);

int	main(int argc, char* argv[])
{
	(void)argv;

	signal(SIGINT, signalHandler);
	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
		return 2;
	}
	Bot	bot;

	bot.launcher_bot(argv[1], argv[2]);

	return (0);
}
