/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Board.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lud-adam <lud-adam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 19:56:11 by lud-adam          #+#    #+#             */
/*   Updated: 2026/06/17 12:03:12 by lud-adam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOARD_H
# define BOARD_H

# include <stddef.h>
# include <stdbool.h>

# include <string>

typedef struct s_board
{
	bool	running;
	bool	alreadySet;
	size_t	grid[5][5];
	size_t	size;
	size_t	empty_case;
}			t_board;

enum e_const
{
	WIN_VALUE = 32
};


class	Board
{
	public:
		// Board(t_board& board);
		t_board&		getBoard();
		std::string&	getBuf();
		int				getSocket() const;
		void			setSocket(int socket);
		void			setBoard(t_board board);
		void			setNick(std::string nick);
		bool			game_loop(t_board *board, char c);
	private:
		// t_board&	_board;
		std::string _nick;
		std::string	_buf;
		int			_socket;

		void	send_grid(t_board* board);

};

void	init_board(t_board* board, size_t size);
bool	fill_start_numbers(t_board* board);
bool	fill_nb_rd_place(t_board* board);

//GAME LOGIC UTILS
void	reset_pos(int* pos);
bool	check_horizontal_game_over(t_board *board);
void	sort_pos(int* pos);
int		get_index(int* pos);
void	fill_pos(int* pos, int index);
bool	have_pos(int* pos);
bool	reverse_line(size_t* line, int size);
bool	reverse_column(t_board* board, int c);

//CHECK
bool	is_game_over(t_board* board);
bool	is_victory(t_board* board);

#endif
