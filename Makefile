MAKEFLAGS += -j

.PHONY : all clean fclean re debug

CC = c++
CC_DEBUG = g++

CFLAGS = -g3 -Wall -Wextra -Werror -MMD -std=c++98
CFLAGS_DEBUG = -Wall -Wextra -MMD -std=c++98 -g -D DEBUG=1

NAME = ircserv
NAME_DEBUG = ircserv_debug

P_SRC = src/
P_SRC_SERVER = $(P_SRC)server/
P_SRC_COMMAND = $(P_SRC)command/

P_OBJ = .obj/
P_OBJ_DEBUG = .obj_debug/

P_INC = inc/
P_INC_SERVER = $(P_INC)server/
P_INC_COMMAND = $(P_INC)command/
P_INC_UTILS = $(P_INC)utils/

INCS = \
	   $(addprefix -I, $(P_INC)) \
	   $(addprefix -I, $(P_INC_SERVER)) \
	   $(addprefix -I, $(P_INC_COMMAND)) \
	   $(addprefix -I, $(P_INC_UTILS)) \

SRC =			main.cpp	\
				Client.cpp	\
				Channel.cpp \
				debug.cpp \

SRC_SERVER =	Server.cpp	\
				ServerSideProcessing.cpp \

SRC_COMMAND = \
				Command.cpp \
				CommandFactory.cpp \
				JoinCommand.cpp \
				PassCommand.cpp \
				NickCommand.cpp \
				UserCommand.cpp \
				PartCommand.cpp \
				ModeCommand.cpp \
				KickCommand.cpp \
				TopicCommand.cpp \
				InviteCommand.cpp \
				PrivmsgCommand.cpp \
				QuitCommand.cpp \
				WhoCommand.cpp \

SRC_REPLY_BUILDER = ReplyBuilder.cpp \

SRC_UTILS = utils.cpp \

SRCS = \
	$(addprefix $(P_SRC), $(SRC)) \
	$(addprefix $(P_SRC), $(SRC_UTILS)) \
	$(addprefix $(P_SRC), $(SRC_REPLY_BUILDER)) \
	$(addprefix $(P_SRC_SERVER), $(SRC_SERVER)) \
	$(addprefix $(P_SRC_COMMAND), $(SRC_COMMAND)) \

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
	@$(CC) $(CFLAGS) $(INCS) -c $< -o $@ && \
	echo "$(Cyan)Compiling $<$(Color_Off)" || \
	echo "$(Red)Error compiling $<$(Color_Off)"

clean:
	rm -rf $(P_OBJ) $(P_OBJ_DEBUG) $(P_OBJ_BOT)

fclean: clean
	rm -f $(NAME) $(NAME_DEBUG) $(NAME_BOT)

re:
	$(MAKE) fclean
	$(MAKE) all

#############################################################################################
#                                                                                           #
#                                           DEBUG                                           #
#                                                                                           #
#############################################################################################

debug: $(NAME_DEBUG)

bot: 
	$(MAKE) -C ./IRC_BOT/

$(NAME_DEBUG): $(OBJS_DEBUG)
	@$(CC_DEBUG) $(CFLAGS_DEBUG) -o $@ $(OBJS_DEBUG) && \
	echo "$(Green)Creating executable $@$(Color_Off)" || \
	echo "$(Red)Error creating $@$(Color_Off)"

$(P_OBJ_DEBUG)%.o: $(P_SRC)%.cpp
	@mkdir -p $(dir $@)
	@$(CC_DEBUG) $(CFLAGS_DEBUG) $(INCS) -c $< -o $@ && \
	echo "$(Cyan)Compiling $< [debug]$(Color_Off)" || \
	echo "$(Red)Error compiling $<$(Color_Off)"

-include $(DEPS)
-include $(DEPS_DEBUG)
-include $(DEPS_BOT)
