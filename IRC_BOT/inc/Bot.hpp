#ifndef	BOT_HPP
# define BOT_HPP

#include <iostream>
#include <map>

#include <netinet/in.h>

#include "Board.hpp"

class Bot
{
	public:
		void							launcher_bot(std::string strPort, std::string password, std::string channel);
		void							setSocket(int socket);
		~Bot();
	private:
		std::map<std::string, t_board>	_nicks;
		int								_socketServer;
		sockaddr_in						_serverAddress;
		std::string						_buf;
		void							connectToServer();
		bool							handleRequest();
		void							sendConnectionToServer(std::string password, std::string channel);
		int								launch_2048(std::string sizeBoard, std::string nick, char c);
};

void									sendPrivateMessage(int socket, std::string nick, std::string content);

#endif
