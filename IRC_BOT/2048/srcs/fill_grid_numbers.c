/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_grid_numbers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lud-adam <lud-adam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 13:51:51 by lud-adam          #+#    #+#             */
/*   Updated: 2026/04/26 16:21:12 by lud-adam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "board.h"

#include <stdlib.h>
#include <time.h>
#include <ncurses.h>

static int	gen_two_or_four();
static int gen_number(int min_nb, int max_nb, int modulo);

bool	fill_start_numbers(t_board* board)
{
	srand(time(0));

	int i = 0;
	
	while (i < 2)
	{
		int nb = gen_two_or_four();
		while (1)
		{
			int x = rand() % (board->size - 1);
			int y = rand() % (board->size - 1);
			if (board->grid[x][y] == 0)
			{
				board->grid[x][y] = nb;
				board->empty_case--;
				i++;
				break ;
			}
		}
	}
	return (true);
}

bool	fill_nb_rd_place(t_board* board)
{
	if (board->empty_case == 0)
		return (false);

	int rd_nb = gen_number(1, board->empty_case , 1);
	int nb = gen_two_or_four();
	size_t x = 0;
	size_t y = 0;

	while (y < board->size && rd_nb != 0)
	{
		while (x < board->size && rd_nb != 0)
		{
			if (board->grid[y][x] == 0)
				rd_nb--;
			if (rd_nb == 0)
				break ;
			x++;
		}
		if (rd_nb == 0)
			break ;
		y++;
		x = 0;
	}
	board->grid[y][x] = nb;
	board->empty_case--;
	return (true);	
}

static int	gen_two_or_four()
{
	int res = 0;
	int nb = rand() % 9;

	if (nb == 0 || nb == 1 || nb == 2 || nb == 3 || nb == 5 || nb == 6 || nb == 7 || nb == 9)
		res = 2;
	else if (nb == 4 || nb == 8)
		res = 4;
	return (res);
}

static int gen_number(int min_nb, int max_nb, int modulo)
{
	int res = 0;
	while (1)
	{
		res = rand() % (max_nb + 1 - min_nb) + min_nb;
		if (res != 0 && res % modulo == 0)
			break ;
	}
	return (res);
}
