/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lud-adam <lud-adam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 19:38:23 by lud-adam          #+#    #+#             */
/*   Updated: 2026/06/17 09:28:23 by lud-adam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "board.hpp"
#include "utils.hpp"

#include <stdio.h>

#include <sstream>

int	launch_2048(std::string sizeBoard)
{
	if (sizeBoard.size() == 1)
	{
		std::stringstream ss(sizeBoard);
		int size;

		ss >> size;
		if (size != 4 && size != 5)
		{
			printf("Wrong board size\n");
			return (1);
		}
		t_board	board;

		init_board(&board,  size);
		fill_start_numbers(&board);
		game_loop(&board);
	}
	else
		printf("Wrong board size\n");
	return (0);
}
