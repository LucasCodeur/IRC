/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   board.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lud-adam <lud-adam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 19:38:54 by lud-adam          #+#    #+#             */
/*   Updated: 2026/06/16 19:38:55 by lud-adam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "board.hpp"

#include <assert.h>
#include <stddef.h>

void	init_board(t_board *board, size_t size)
{
	assert((size >= 4 && size <= 5) && "Invalid board size");
	board->size = size;
	for (size_t i = 0; i < size; i++)
		for (size_t j = 0; j < size; j++)
			board->grid[i][j] = 0;
	board->empty_case = size * size;
}
