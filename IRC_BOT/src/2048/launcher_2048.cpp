/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launcher_2048.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lud-adam <lud-adam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 19:38:23 by lud-adam          #+#    #+#             */
/*   Updated: 2026/06/17 12:23:49 by lud-adam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Board.hpp"

#include "utils.hpp"
#include "Bot.hpp"

#include <stdio.h>

#include <sstream>


int	Bot::launch_2048(std::string sizeBoard, std::string nick)
{
	if (sizeBoard.size() == 1)
	{
		std::stringstream ss(sizeBoard);
		int size;

		ss >> size;
		if (size != 4 && size != 5)
		{
			sendPrivateMessage(this->_socketServer, nick, "Wrong board size : 4 or 5");
			return (1);
		}

		Board	board;
		board.setSocket(this->_socketServer);
		board.setNick(nick);

		init_board(&board._board,  size);
		fill_start_numbers(&board._board);
		board.game_loop(&board._board);
	}
	else
		sendPrivateMessage(this->_socketServer, nick, "Wrong board size : 4 or 5");
	return (0);
}
