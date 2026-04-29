#ifndef SERVER_HPP
#define SERVER_HPP

#include <map>
#include <ostream>
#include <string>
#include "Channel.hpp"
#include "Client.hpp"
#include "Command.hpp"

class Server
{
	private:
	int								_port;
	int								_fd;
	std::string						_serverName;
	std::string						_password;
	std::map<int, Client>			_clients;
	std::map<std::string, Channel*>	_channels;

	public:
	// CONSTRUCTOR
	Server();
	Server(int port, const std::string &password);
	~Server();

	// GETTERS
	int getPort() const;
	int getFd() const;
	std::string const &getServerName() const;
	std::string const &getPassword() const;
	std::map<int, Client> const &getClientmap() const;
	std::map<std::string, Channel*> const &getChanelmap() const;
	
	// METHODS
	void handleCommand(Command const &cmd);

	private:
	Server(Server const &original);
	Server &operator=(Server const &other);
	void handleJoin(Command const &cmd);
};
std::ostream &operator<<(std::ostream &o, const Server &obj);

#endif // !SERVER_HPP
