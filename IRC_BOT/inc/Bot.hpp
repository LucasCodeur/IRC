#ifndef	BOT_HPP
# define BOT_HPP

#include <iostream>
#include <netinet/in.h>

class Bot
{
	public:
		void	launcher_bot(std::string strPort, std::string password, std::string channel);
		void	setSocket(int socket);

	private:
		int			_socketServer;
		sockaddr_in _serverAdress;
		std::string _buf;
		void		connectToServer();
		std::string	extractCommand(std::string& buffer);
		bool		handleRequest();
		bool		receiveData();
		void		display_buffer(std::string& buffer);
		bool		convertPort(std::string port, int& portToSet);
		void		sendConnectionToServer(std::string password, std::string channel);
		void		setNonBlocking(int sock);
		int			launch_2048(std::string sizeBoard, std::string nick);
};

void		sendPrivateMessage(int socket, std::string nick, std::string content);
void		sendData(int socket, std::string message);
bool		splitPrivmsg(std::string strCommand, std::string& nick, std::string& content);

#endif
