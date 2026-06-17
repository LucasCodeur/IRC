/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Board.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lud-adam <lud-adam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 19:38:54 by lud-adam          #+#    #+#             */
/*   Updated: 2026/06/17 12:21:17 by lud-adam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Board.hpp"

#include <assert.h>
#include <stddef.h>
#include <iostream>

void	init_board(t_board *board, size_t size)
{
	assert((size >= 4 && size <= 5) && "Invalid board size");
	board->size = size;
	for (size_t i = 0; i < size; i++)
		for (size_t j = 0; j < size; j++)
			board->grid[i][j] = 0;
	board->empty_case = size * size;
}

t_board&	Board::getBoard()
{
	return (this->_board);
}

std::string&	Board::getBuf()
{
	return (this->_buf);
}

int		Board::getSocket() const
{
	return (this->_socket);
}

void	Board::setSocket(int socket)
{
	this->_socket = socket;
	std::cout << "socket: " << this->_socket << std::endl; 
}

void	Board::setNick(std::string nick)
{
	this->_nick = nick;
}
