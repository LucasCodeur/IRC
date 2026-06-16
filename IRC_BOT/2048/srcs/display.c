/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lud-adam <lud-adam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 19:34:01 by lud-adam          #+#    #+#             */
/*   Updated: 2026/06/16 19:50:38 by lud-adam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "board.h"

#include <stdio.h>

void print_simple_grid(t_board* board) 
{
    for (size_t i = 0; i < board->size; i++)
    {
		if (board->size == 4)
			printf("+---+---+---+---+\n");
		else
			printf("+---+---+---+---+---+\n");
        
        printf("|");
        for (size_t j = 0; j < board->size; j++)
        {
            if (board->grid[i][j] == 0)
                printf("   |");
            else
                printf(" %zu |", board->grid[i][j]);
        }
        printf("\n");
    }
	if (board->size == 4)
		printf("+---+---+---+---+\n");
	else
		printf("+---+---+---+---+---+\n");
}
