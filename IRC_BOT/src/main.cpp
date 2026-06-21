#include "Bot.hpp"

#include <iostream>

#include <csignal>

void signalHandler(int signum);

int	main(int argc, char* argv[])
{
	(void)argv;

	signal(SIGINT, signalHandler);
	if (argc != 4)
	{
		std::cerr << "Usage: " << argv[0] << " <port> <password> <channel>" << std::endl;
		return (1);
	}

	Bot	bot;
	bot.launcher_bot(argv[1], argv[2], argv[3]);

	return (0);
}

