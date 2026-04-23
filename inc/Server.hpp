#ifndef SERVER_HPP
#define SERVER_HPP

#include <map>
#include <ostream>
#include <string>
#include "Channel.hpp"
#include "Command.hpp"
#include "Client.hpp"

class Server
{
	private:
	int								_port;
	int								_fd;
	std::string						_serverName;
	std::string						_password;
	std::map<int, Client>			_clients; //int for the fd of the client
	std::map<std::string, Channel>	_channels; // string for the name of the chanel

	public:
	// CONSTRUCTOR
	Server();
	~Server();
	Server(Server const &original);

	// OPERATOR
	Server &operator=(Server const &other);

	// GETTERS

	// METHODS
	void handleCommand(Command const &cmd);
	void test();
};
std::ostream &operator<<(std::ostream &o, const Server &obj);


#endif // !SERVER_HPP