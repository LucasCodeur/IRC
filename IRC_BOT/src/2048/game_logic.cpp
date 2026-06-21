/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_logic.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lud-adam <lud-adam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 17:18:51 by lud-adam          #+#    #+#             */
/*   Updated: 2026/06/17 13:58:41 by lud-adam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Board.hpp"
#include "utils.hpp"
#include "Bot.hpp"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <sys/socket.h>
#include <string.h>
#include <fcntl.h>

#include <sstream>

#define BUFFER_SIZE 2048

static bool	move_side(t_board* board, bool move_right);
static bool	move_verticality(t_board* board, bool move_bottom);

bool	Board::game_loop(t_board *board, char c)
{
	board->running = true;
	sendPrivateMessage(this->_socket, this->_nick, "Commande (a=left, d=right, w=up, s=down, q=quit) :");

	if (c == 'a')
		move_side(board, false);
	else if (c == 'd')
		move_side(board, true);
	else if (c == 'w')
		move_verticality(board, false);
	else if (c == 's')
		move_verticality(board, true);
	else if (c == 'q')
	{
		board->running = false;
		return (false);
	}
	if (c == 'a' || c == 'd' || c == 'w' || c == 's')
		fill_nb_rd_place(board);
	if (is_victory(board) == true)
	{
		sendPrivateMessage(this->_socket, this->_nick, "Victory");
		board->running = false;
		send_grid(board);
		return (false);
	}
	if (is_game_over(board) == true)
	{
		sendPrivateMessage(this->_socket, this->_nick, "Game over");
		board->running = false;
		send_grid(board);
		return (false);
	}
	send_grid(board);
	return (true);
}

void	Board::send_grid(t_board* board) 
{
	std::string grid;	
	for (size_t i = 0; i < board->size; i++)
	{
		if (board->size == 4)
			grid ="+---+---+---+---+";
		else
			grid = "+---+---+---+---+---+";
		sendPrivateMessage(this->_socket, this->_nick, grid);
		grid = "|";
		for (size_t j = 0; j < board->size; j++)
		{
			if (board->grid[i][j] == 0)
				grid +="   |";
			else
			{
				std::stringstream convert;
				convert << board->grid[i][j];
				std::string		nb = convert.str();
				grid += " ";
				grid += nb;
				grid += " |";
			}
		}
		sendPrivateMessage(this->_socket, this->_nick, grid);
	}
	if (board->size == 4)
		grid ="+---+---+---+---+";
	else
		grid = "+---+---+---+---+---+";
	sendPrivateMessage(this->_socket, this->_nick, grid);
}

static bool	tighten_side_grid(t_board* board, bool move_right);
static bool	add_line(t_board *board, int l, bool move_right);

bool	move_side(t_board* board, bool move_right)
{
	bool	res = false;

	res = tighten_side_grid(board, move_right);
	for (size_t i  = 0; i < board->size; i++)
	{
		if (add_line(board, i, move_right) == true)
			res = true;
	}
	if (res == true)
		tighten_side_grid(board, move_right);
	else
		res = tighten_side_grid(board, move_right);
	return (res);
}

static bool	tighten_side_grid(t_board* board, bool move_right)
{
	bool		res = false;
	int			pos[board->size];
	size_t		x = 0;
	size_t		y = 0;

	memset(pos, -1, sizeof(int) * board->size);
	while (y < board->size)
	{
		if (move_right == true)
			reverse_line(board->grid[y], board->size);
		while (x < board->size)
		{
			if (board->grid[y][x] == 0)
				fill_pos(pos, x);		
			else if (board->grid[y][x] != 0 && have_pos(pos) == true)
			{
				int old_x = get_index(pos);
				board->grid[y][old_x] = board->grid[y][x];
				board->grid[y][x] = 0;
				fill_pos(pos, x);
				res = true;
			}
			x++;
		}
		if (move_right == true)
			reverse_line(board->grid[y], board->size);
		y++;
		reset_pos(pos);
		x = 0;
	}
	return (res);
}

static bool	tighten_verticality_grid(t_board* board, bool move_bottom);
static bool	add_column(t_board *board, int c, bool move_bottom);

bool	move_verticality(t_board* board, bool move_bottom)
{
	bool	res = false;

	res = tighten_verticality_grid(board, move_bottom);
	for (size_t i  = 0; i < board->size; i++)
	{
		if (add_column(board, i, move_bottom) == true)
			res = true;
	}
	if (res == true)
		tighten_verticality_grid(board, move_bottom);
	else
		res = tighten_verticality_grid(board, move_bottom);
	return (res);
	
}

static bool	tighten_verticality_grid(t_board* board, bool move_bottom)
{
	bool		res = false;
	int			pos[board->size];
	size_t		x = 0;
	size_t		y = 0;

	memset(pos, -1, sizeof(int) * board->size);
	while (x < board->size)
	{
		if (move_bottom == true)
			reverse_column(board, x);
		while (y < board->size)
		{
			if (board->grid[y][x] == 0)
				fill_pos(pos, y);		
			else if (board->grid[y][x] != 0 && have_pos(pos) == true)
			{
				int old_y = get_index(pos);
				board->grid[old_y][x] = board->grid[y][x];
				board->grid[y][x] = 0;
				fill_pos(pos, y);
				res = true;
			}
			y++;
		}
		if (move_bottom == true)
			reverse_column(board, x);
		x++;
		reset_pos(pos);
		y = 0;
	}
	return (res);
	
}

static bool add_line(t_board *board, int l, bool move_right)
{
	bool	res = false;

	if (move_right == false)
	{
		size_t i = 1;	

		while (i < board->size)
		{
			if (board->grid[l][i] != 0 && board->grid[l][i] == board->grid[l][i - 1])
			{
				board->empty_case++;
				board->grid[l][i - 1] *= 2;
				board->grid[l][i] = 0;
				res = true;
			}
			i++;
		}
	}
	else
	{
		size_t i = board->size - 1;	

		while (i > 0)
		{
			if (board->grid[l][i] != 0 && board->grid[l][i] == board->grid[l][i - 1])
			{
				board->empty_case++;
				board->grid[l][i] *= 2;
				board->grid[l][i - 1] = 0;
				res = true;
			}
			i--;
		}
	}
	return (res);
}

static bool add_column(t_board *board, int c, bool move_bottom)
{
	bool	res = false;

	if (move_bottom == false)
	{
		size_t i = 1;	

		while (i < board->size)
		{
			if (board->grid[i][c] != 0 && board->grid[i][c] == board->grid[i - 1][c])
			{
				board->empty_case++;
				board->grid[i - 1][c] *= 2;
				board->grid[i][c] = 0;
				res = true;
			}
			i++;
		}
	}
	else
	{
		size_t i = board->size - 1;	

		while (i > 0)
		{
			if (board->grid[i][c] != 0 && board->grid[i][c] == board->grid[i - 1][c])
			{
				board->empty_case++;
				board->grid[i][c] *= 2;
				board->grid[i - 1][c] = 0;
				res = true;
			}
			i--;
		}
	}
	return (res);
}
