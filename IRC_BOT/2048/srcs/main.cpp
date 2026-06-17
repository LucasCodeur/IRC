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

int	main(int argc, char **argv)
{
	if (argc == 2 && ft_strlen(argv[1]) == 1)
	{
		int size = argv[1][0] - '0';
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
	return (0);
}
