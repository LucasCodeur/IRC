#ifndef SERVER_HPP
#define SERVER_HPP

#include <map>
#include <ostream>
#include <string>
#include "Channel.hpp"
#include "Client.hpp"

class Server
{
	private:
	int								_port;
	int								_fd;
	std::string						_password;
	std::map<std::string, Client>	_client;
	std::map<std::string, Channel>	_channel;

	public:
	// CONSTRUCTOR
	Server();
	~Server();
	Server(Server const &original);

	// OPERATOR
	Server &operator=(Server const &other);
};
std::ostream &operator<<(std::ostream &o, const Server &obj);


#endif // !SERVER_HPP