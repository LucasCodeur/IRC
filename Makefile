# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: enchevri <enchevri@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/04/13 11:36:21 by lud-adam          #+#    #+#              #
#    Updated: 2026/05/05 17:00:23 by enchevri         ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

MAKEFLAGS += -j

.PHONY : all clean fclean re debug
CC = c++
CC_DEBUG = g++
CFLAGS = -Wall -Wextra -Werror -MMD -std=c++98
CFLAGS_DEBUG = -Wall -Wextra -MMD -std=c++98 -g3 -D DEBUG=1
NAME = ircserv
NAME_DEBUG = ircserv_debug
P_SRC = src/
P_OBJ = .obj/
P_OBJ_DEBUG = .obj_debug/
P_INC = inc/

SRC = main.cpp \

SRCS = \
	$(addprefix $(P_SRC), $(SRC)) \

INCS = $(addprefix $(P_INC), $(INC)) \

OBJS = $(subst $(P_SRC), $(P_OBJ), $(SRCS:.cpp=.o))
OBJS_DEBUG = $(subst $(P_SRC), $(P_OBJ_DEBUG), $(SRCS:.cpp=.o))

DEPS = $(OBJS:%.o=%.d)
DEPS_DEBUG = $(OBJS_DEBUG:%.o=%.d)

Color_Off=\033[0m
Green=\033[0;32m
Red=\033[0;31m
Cyan=\033[0;36m

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) -o $@ $(OBJS) && \
	echo "$(Green)Creating executable $@$(Color_Off)" || \
	echo "$(Red)Error creating $@$(Color_Off)"

$(P_OBJ)%.o: $(P_SRC)%.cpp
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -I $(P_INC) -c $< -o $@ && \
	echo "$(Cyan)Compiling $<$(Color_Off)" || \
	echo "$(Red)Error compiling $<$(Color_Off)"

clean:
	rm -rf $(P_OBJ) $(P_OBJ_DEBUG)

fclean: clean
	rm -f $(NAME) $(NAME_DEBUG)

re:
	$(MAKE) fclean
	$(MAKE) all

#############################################################################################
#                                                                                           #
#                                           DEBUG                                           #
#                                                                                           #
#############################################################################################

debug: $(NAME_DEBUG)

$(NAME_DEBUG): $(OBJS_DEBUG)
	@$(CC_DEBUG) $(CFLAGS_DEBUG) -o $@ $(OBJS_DEBUG) && \
	echo "$(Green)Creating executable $@$(Color_Off)" || \
	echo "$(Red)Error creating $@$(Color_Off)"

$(P_OBJ_DEBUG)%.o: $(P_SRC)%.cpp
	@mkdir -p $(dir $@)
	@$(CC_DEBUG) $(CFLAGS_DEBUG) -I $(P_INC) -c $< -o $@ && \
	echo "$(Cyan)Compiling $< [debug]$(Color_Off)" || \
	echo "$(Red)Error compiling $<$(Color_Off)"

-include $(DEPS)
-include $(DEPS_DEBUG)
