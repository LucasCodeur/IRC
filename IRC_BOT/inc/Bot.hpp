#ifndef	BOT_HPP
# define BOT_HPP

#include <iostream>
#include <netinet/in.h>

class Bot
{
	public:
		void	launcher_bot(std::string strPort, std::string password);
		void	setSocket(int socket);

	private:
		int			_socketServer;
		sockaddr_in _serverAdress;
		std::string _buf;
		void		connectToServer();
		void		sendData(std::string data);
		std::string	extractCommand(std::string& buffer);
		bool		handleRequest();
		bool		receiveData();
		void		display_buffer(std::string& buffer);
		bool		convertPort(std::string port, int& portToSet);
		void		sendConnectionToServer(std::string password);
		void		setNonBlocking(int sock);
};

#endif
