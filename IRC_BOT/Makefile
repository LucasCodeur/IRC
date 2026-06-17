MAKEFLAGS += -j

.PHONY : all clean fclean re debug

CXX = c++
CXX_DEBUG = g++

CXXFLAGS = -Wall -Wextra -Werror -MMD -std=c++98
CXXFLAGS_DEBUG = -Wall -Wextra -MMD -std=c++98 -g3 -D DEBUG=1

NAME = ircbot
NAME_DEBUG = ircbot_debug 

P_SRC = src/
P_SRC_SERVER = $(P_SRC)

P_OBJ = .obj/
P_OBJ_DEBUG = .obj_debug/

P_INC = inc/

P_SRC_2048 = src/2048/

SRC = main.cpp \
	  Bot.cpp \

SRC_2048 = launcher_2048.cpp \
		   board.cpp \
		   display.cpp \
		   game_logic.cpp \
		   check_game_over.cpp \
		   fill_grid_numbers.cpp \
		   game_logic_utils.cpp \
		   utils.cpp

SRCS = \
	$(addprefix $(P_SRC), $(SRC)) \
	$(addprefix $(P_SRC_SERVER), $(SRC_SERVER)) \
	$(addprefix $(P_SRC_2048), $(SRC_2048)) \

OBJS = $(subst $(P_SRC), $(P_OBJ), $(SRCS:.cpp=.o)) \

OBJS_DEBUG = $(subst $(P_SRC), $(P_OBJ_DEBUG), $(SRCS:.cpp=.o)) \

DEPS = $(OBJS:%.o=%.d)

Color_Off=\033[0m
Green=\033[0;32m
Red=\033[0;31m
Cyan=\033[0;36m

all: $(NAME)

$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) -o $@ $(OBJS) && \
	echo "$(Green)Creating executable $@$(Color_Off)" || \
	echo "$(Red)Error creating $@$(Color_Off)"

$(P_OBJ)%.o: $(P_SRC)%.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -I $(P_INC) -c $< -o $@ && \
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
	@$(CXX_DEBUG) $(CXXFLAGS_DEBUG) -o $@ $(OBJS_DEBUG) && \
	echo "$(Green)Creating executable $@$(Color_Off)" || \
	echo "$(Red)Error creating $@$(Color_Off)"

$(P_OBJ_DEBUG)%.o: $(P_SRC)%.cpp
	@mkdir -p $(dir $@)
	@$(CXX_DEBUG) $(CXXFLAGS_DEBUG) -I $(P_INC) -c $< -o $@ && \
	echo "$(Cyan)Compiling $< [debug]$(Color_Off)" || \
	echo "$(Red)Error compiling $<$(Color_Off)"

-include $(DEPS)
-include $(DEPS_DEBUG)
