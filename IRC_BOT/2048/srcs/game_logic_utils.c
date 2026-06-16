/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_logic_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lud-adam <lud-adam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 16:06:53 by lud-adam          #+#    #+#             */
/*   Updated: 2026/04/26 16:11:55 by lud-adam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "board.h"

bool	reverse_column(t_board *board, int c)
{
	size_t i = 0;
	while (i < board->size / 2)
	{
		int temp = board->grid[i][c];
		board->grid[i][c] = board->grid[board->size - i - 1][c];
		board->grid[board->size - i - 1][c] = temp;
		i++;
	}
	return (true);
}

bool	reverse_line(size_t* line, int size)
{
	int i = 0;
	while (i < size / 2) 
	{
		int temp = line[i];
		line[i] = line[size - i - 1];
		line[size - i - 1] = temp;
		i++;
	}
	return (true);
}

void sort_pos(int* pos);

void	fill_pos(int* pos, int index)
{
	for (int i = 0; i < 3; i++)
	{
		if (pos[i] == -1)
		{
			pos[i] = index;
			sort_pos(pos);
			break ;
		}
	}
}

int get_index(int* pos)
{
	for (int i = 0; i < 3; i++)
	{
		if (pos[i] != -1)
		{
			int index = pos[i];
			pos[i] = -1;
			return (index);
		}
	}
	return (0);
}

void sort_pos(int* pos)
{
	int i = 1;

	while (i < 3)
	{
		if (pos[i - 1] > pos[i])
		{
			int temp = pos[i];
			pos[i] = pos[i - 1];
			pos[i - 1] = temp;
		}
		i++;
	}
}

bool	have_pos(int *pos)
{
	int i = 0;

	while (i < 3)
	{
		if (pos[i] != -1)
			return (true);
		i++;
	}
	return (false);
}

void	reset_pos(int* pos)
{
	int i = 0;

	while (i < 3)
	{
		pos[i] = -1;
		i++;
	}
}
