/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lud-adam <lud-adam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 10:56:52 by lud-adam          #+#    #+#             */
/*   Updated: 2026/06/16 10:57:52 by lud-adam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

#include <iostream>

#include <csignal>

static void signalHandler(int signum);
int stopVar = false;

int	main(int argc, char* argv[])
{
	Bot	bot;
	(void)argv;

	signal(SIGINT, signalHandler);
	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
		return 2;
	}
	bot.launcher_bot(argv[1], argv[2]);

	return (0);
}

static void signalHandler(int signum)
{
	(void)signum;
	std::cerr << "shutting down server..." << std::endl;
	stopVar = true;
}
