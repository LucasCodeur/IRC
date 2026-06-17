# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lud-adam <lud-adam@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/06/16 19:39:28 by lud-adam          #+#    #+#              #
#    Updated: 2026/06/16 19:39:33 by lud-adam         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			:= 2048
BUILD_DIR		:= .build
SRC_DIR			:= srcs
INCLUDE_DIR		:= includes
CXX				:= c++
CXXFLAGS		:= -Wall -Wextra -Werror
CPPFLAGS		:= -MMD -MP -I $(INCLUDE_DIR)

SRCS			:=			\
	main.cpp				\
	board.cpp				\
	game_logic.cpp			\
	fill_grid_numbers.cpp	\
	display.cpp				\
	check_game_over.cpp		\
	game_logic_utils.cpp	\
	utils.cpp				\

SRCS			:= $(SRCS:%=$(SRC_DIR)/%)
OBJS			:= $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEPS			:= $(OBJS:.o=.d)
RM				:= rm -rf

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(OBJS) -o $(NAME)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

force:

clean:
	$(RM) $(BUILD_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

run: all
	./$(NAME)

debug:
	$(MAKE) re CXXFLAGS="-Wall -Wextra -g3 -D DEBUG=1"

-include $(DEPS)

.PHONY: all clean fclean re run debug
