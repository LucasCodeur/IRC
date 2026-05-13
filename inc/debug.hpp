#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <iostream>
#ifndef DEBUG
#define DEBUG 0
#endif

#define RESET		"\033[0m"
#define RED		"\033[31m"
#define GREEN		"\033[32m"
#define YELLOW		"\033[33m"
#define BLUE		"\033[34m"
#define MAGENTA		"\033[35m"
#define CYAN		"\033[36m"
#define WHITE		"\033[37m"
#define DBUG		YELLOW "DEBUG " RESET

#define PRINT(x, color, newline) \
    std::cout << color << x << RESET <<newline;

#endif
