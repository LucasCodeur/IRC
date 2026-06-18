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
#include <errno.h>
#include <string>
#include <sys/socket.h>
#include <string.h>
#include <fcntl.h>

#include <sstream>

#define BUFFER_SIZE 2048

static bool	move_side(t_board* board, bool move_right);
static bool	move_verticality(t_board* board, bool move_bottom);
bool		receiveData(int socket, std::string& buf);

static char ft_getchar(int socket, std::string nick, std::string& buf);

bool	Board::game_loop(t_board *board, char c)
{
	board->running = true;
	send_grid(board);
	sendPrivateMessage(this->_socket, this->_nick, "Commande (a=left, d=right, w=up, s=down, q=quit) :");

	if (c == 'a')
	{
		std::cout << "inside a" << std::endl;
		move_side(board, false);
	}
	else if (c == 'd')
		move_side(board, true);
	else if (c == 'w')
	{
		std::cout << "inside w" << std::endl;
		move_verticality(board, false);
	}
	else if (c == 's')
		move_verticality(board, true);
	else if (c == 'q')
		board->running = false;
	fill_nb_rd_place(board);

	// while (c != '\n' && c != EOF)
	// 	c = ft_getchar(this->_socket, this->_nick, this->_buf);

	// sendPrivateMessage(this->_socket, this->_nick, "\n");
	if (is_victory(board) == true)
	{
		sendPrivateMessage(this->_socket, this->_nick, "Victory");
		board->running = false;
		return (false);
	}
	if (is_game_over(board) == true)
	{
		sendPrivateMessage(this->_socket, this->_nick, "Game over");
		board->running = false;
		return (false);
	}
	return (false);
}

static char ft_getchar(int socket, std::string nick, std::string& buf)
{
	char c = '\0';
	// while (1)
	// {
		if (receiveData(socket, buf) == false)
			sendPrivateMessage(socket, nick, "Commande (a=left, d=right, w=up, s=down, q=quit) :");
	// 	else
	// 		break ;
	// }
	if (buf.empty() == false)	
	{
		std::string nick;
		std::string content;
		if (splitPrivmsg(buf, nick, content) == false)
			return ('\0');
		c = content[0];
		std::cout << "c : " << c << std::endl;
		buf.erase(0, buf.size());
	}
	return (c);
}

void setBlocking(int sock)
{
	int result;
	int flags;

	flags = ::fcntl(sock, F_GETFL, 0);
	if (flags == -1)
		throw (std::runtime_error("fcntl failed"));
	flags &= ~O_NONBLOCK;
	result = fcntl(sock , F_SETFL , flags);
	if (result == -1)
	{
		throw (std::runtime_error("fcntl failed"));
	}
}

bool	receiveData(int socket, std::string& buf)
{
	// std::cout << "Inside receive data: " << stopVar << std::endl;
	//
	setBlocking(socket);
	int	bytes_read;
	char	buffer[BUFFER_SIZE] = {"0"};

	memset(buffer, 0, BUFFER_SIZE);
	bytes_read = recv(socket, buffer, sizeof(buffer), 0);
	if (bytes_read <= 0)
	{
		if (bytes_read == 0 || ((bytes_read == -1) && (errno != EAGAIN && errno != EWOULDBLOCK)))
		{
			// std::cout << "Inside bytes_read <= 0" << std::endl;
			return (false);
		}
	}
	std::cout << "buffer :" << buffer << std::endl;
	buffer[bytes_read] = '\0';
	if (strlen(buffer) != 0 && buffer[0] != '\0')
	{
		buf += buffer;
	}
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
		// grid += "\n";
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

	ft_memset(pos, -1, sizeof(int) * board->size);
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

	ft_memset(pos, -1, sizeof(int) * board->size);
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
