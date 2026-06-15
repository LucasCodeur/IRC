#include "debug.hpp"
#include "Exceptions.hpp"
#include "Command.hpp"
#include "Server.hpp"

#include <cctype>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>
#include <utility>

#include <iostream>
#include <string>

int stopVar = false;

void signalHandler(int signum)
{
	(void)signum;
	std::cerr << "shutting down server..." << std::endl;
	stopVar = true;
}

/**
 * @brief set up the server and launch it.
 * @return true if no errors occur.
 */
bool	Server::launcherServer(std::string port, std::string password)
{
	try 
	{
		convertPort(port, this->_port);
		this->check_password(password);
		this->_password = password;
	}
	catch (std::exception &e)
	{
		PRINT(e.what(), RED, "\n");
		return (false);
	}
	this->_server_sock = this->createSocket(AF_INET, SOCK_STREAM, DEFAULT);
	this->setSocketOption(this->_server_sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT);
	this->setAddr();
	this->bindSocket();
	this->listenSocket(MAX_WAITING_LIST);
	this->setEpoll(DEFAULT);
	this->controlEpoll(EPOLL_CTL_ADD, this->_server_sock, &this->_ev[0]);
	this->listenConnexionsEpoll();

	return (true);
}

/**
 * @brief infinite loop that allows receiving and sending data between the different sockets contains in the ready list of epoll.
 * @return
 */
void	Server::listenConnexionsEpoll(void)
{
	socklen_t addrlen = sizeof(this->_addr);
	int nfds = 1;
		
	while (stopVar == false)
	{
		std::map<int, Client*>::const_iterator it = this->_clients.begin();
		std::cout << "LIST CLIENTS:" << std::endl;
		while (it != this->_clients.end())
		{
			Client *client = it->second;
			std::cout << "client " << client->getNickname() << " fd : " << client->getFd() << std::endl;
			++it;
		}
		try
		{
			nfds = this->epollWaitOperation(MAX_EVENTS, TIMEOUT);
		}
		catch (std::exception &e)
		{
			continue;
		}
		for (int n = 0; n < nfds; n++)
		{

			if (this->_ev[n].data.fd == this->_server_sock)
			{
				int new_fd = this->acceptConnexion(&addrlen);
				this->setNonBlocking(new_fd);
				this->_ev[n + 1].events = EPOLLIN | EPOLLOUT;
				this->_ev[n + 1].data.fd = new_fd;
				this->controlEpoll(EPOLL_CTL_ADD, new_fd, &this->_ev[n + 1]);
				std::map<int, Client*>::const_iterator it = this->_clients.find(new_fd);
					if (it == this->_clients.end())
					{
						Client* temp = new Client;
						if (this->getPassword().empty() == true)
							temp->getAuthstate().setPasswordReceived(true);
						temp->setFd(new_fd);
						this->_clients.insert(std::pair<int, Client*>(new_fd, temp));
					}
				continue;
			}

			if (this->_ev[n].events & EPOLLIN)
			{
				Client *client = this->getClient(this->_ev[n].data.fd);
				this->receiveData(this->_ev[n].data.fd);
				this->handleRequest(*client);
			}

			if (this->_ev[n].events & EPOLLOUT)
			{
				Client *client = this->getClient(this->_ev[n].data.fd);
				if (client == NULL)
					continue;
				std::string	&clientInputBuffer = client->getClientInputBuffer();
				if (!clientInputBuffer.empty())
				{
					std::cout << "sending to client : " << clientInputBuffer << std::endl;
					try
					{
						sendData(this->_ev[n].data.fd, clientInputBuffer);
					}
					catch (std::exception& e)
					{
						std::cout << "Caught: " << e.what() << std::endl;
						return ;
					}
				}
			}
		}
	}
}

/**
 * @brief wrapper function of socket(), allows creating a socket.
 * @param domain, integer that allows specifying communication domain in order to choose the protocol.
 * @param type_communication, TCP or UDP.
 * @param protocol value to choose the protocol (IP).
 * @return socket descriptor.
 */
int	Server::createSocket(int domain, int type_communication, int protocol)
{
	int sockfd = socket(domain, type_communication, protocol);
	if (sockfd < 0)
		throw socketFailed();
	return (sockfd);
}

/**
 * @brief wrapper function of setsockopt(), allows setting options on the referred socket.
 * @param socket_fd file descrptor created using the socket() function.
 * @param level, allows setting which protocol options have to be set. 
 * @param option_name, name of the option have to be set.
 * @return
 */
void	 Server::setSocketOption(int socket_fd, int level, int option_name)
{
	if (setsockopt(socket_fd, level, option_name, &this->_opt, sizeof(this->_opt)) < 0)
		throw setSocketOptionFailed();
}

/**
 * @brief wrapper function of bind(), allows binding the socket to the adress and port number specified in addr.
 * @return
 */
void	Server::bindSocket(void)
{
	if (bind(this->_server_sock, reinterpret_cast<sockaddr*>(&this->_addr), sizeof(this->_addr)) < 0)
		throw bindFailed();
}

/**
 * @brief wrapper function of listen(), puts the server socket in a passive mode,
 * where it waits for the client to approach the serve to make a connection.
 * @param sizeWaitingList, define the maximum length to which the queue of pending connections 
 * for sockfd may grow.
 * @return
 */
void	Server::listenSocket(int sizeWaitingList)
{
		if (listen(this->_server_sock, sizeWaitingList) < 0)
		throw listenSocketFailed();
}

/**
 * @brief wrapper function of epoll_create1(), allows the "watching" of multiple file descriptor.
 * @param option allow to get different behavior, 0 is default.
 * @return
 */
void	Server::setEpoll(int option)
{
	this->_epollfd = epoll_create1(option);
	if (this->_epollfd == -1)
		throw epollCreateFailed();
	this->_ev[0].events = EPOLLIN;
	this->_ev[0].data.fd = this->_server_sock;
}

/**
 * @brief wrapper function of epoll_ctl(), control inferface for an epoll file descriptor, allows to add, modify
 * or remove entries in the interest list of the epoll().
 * @param op request that the operation op be performed for the target file descriptor.
 * @param fd file descriptor targeted, in order to apply an operation on it. 
 * @param event, data structure that contains information about possible events with epoll.
 * @return
 */
void	Server::controlEpoll(int op, int fd, struct epoll_event* event)
{
	if (epoll_ctl(this->_epollfd, op, fd, event) < 0)
		throw controlEpollFailed();
}

/**
 * @brief wrapper function of accept(), allowing it to extracts the first connection request from the queue of pending connections for the listening socket.
 * @param addrlen  pointer to a variable that specifies the length of the adress structure.
 * @return new file descriptor referring to the first connection request from the queue of pending connections for the listening socket.
 */
int	Server::acceptConnexion(socklen_t* addrlen)
{
	int fd = accept(this->_server_sock, (struct sockaddr *)&this->_addr, addrlen);
	if (fd < 0)
		throw acceptFailed(); 
	return (fd);
}

/**
 * @brief wrapper function of epoll_wait, wait for I/O events, block the current thread if there is no event available.
 * @param max_events the maximum number of events that might be returned.
 * @param timeout arguments specicies the number of milliseconds that epolserverl_wait() will block. 
 * We can see that like an operation in order to extract element inside the ready event list of epoll.
 * @return number of file descriptors ready for the requested I/O or -1 of an error occurs.
 */
int	Server::epollWaitOperation(int max_events, int timeout)
{
	int nfds = epoll_wait(this->_epollfd, &this->_ev[0], max_events, timeout);
	if (nfds < 0)
		throw epollWaitFailed();
	return (nfds);
}

/**
 * @brief wrapper function of send(), allowing it to send data by the indicated socket.
 * @param data, data to send.
 * @param fd file descriptor where data will be sent.
 * @return
 */
void	Server::sendData(int fd, std::string &data)
{
	std::cerr << DBUG YELLOW << "sending to client: " << data << RESET << std::endl;
	if (send(fd, data.c_str(), strlen(data.c_str()), 0) < 0)
		throw sendFailed();
	data = "";
}

void	Server::writeInBuffer(Client *client, std::string data)
{
	std::cerr << "writing in buffer: " << data << std::endl;
	std::cerr << "writing to client: " << client->getNickname() << " fd : " << client->getFd() << std::endl;
	client->addToBuffer(data);
}

/**
 * @brief function to set up the behavior of the socket.
 * @return
 */
void	Server::setAddr(void)
{
	this->_addr.sin_family = AF_INET;
	this->_addr.sin_addr.s_addr = INADDR_ANY;
	this->_addr.sin_port = htons(this->_port);
}

/**
* @brief wrapper function to fcntl(), allowing to set up the socket in a non blocking-mode.
* @return
*/
void Server::setNonBlocking(int sock)
{
	int result;
	int flags;

	flags = ::fcntl(sock, F_GETFL, 0);
	if (flags == -1)
		throw setnonblockingFailed();
	flags |= O_NONBLOCK;
	result = fcntl(sock , F_SETFL , flags);
	if (result == -1)
	{
		throw setnonblockingFailed();
	}
}

Server::Server()
	:
	  _port(0),
	  _fd(-1),
	  _serverName("ircserv"),
	  _password(""),
	  _opt(1)
{
	if (DEBUG == 1)
		std::cout << DBUG GREEN "Server created: " RESET << *this << std::endl;
}


Server::~Server()
{
	for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
		delete it->second;
	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
		delete it->second;
	if (DEBUG == 1)
		std::cout << DBUG RED "Server destroyed: " RESET << *this << std::endl;
}


int Server::getPort() const
{
	return (this->_port);
}

int Server::getFd() const
{
	return (this->_fd);
}

int Server::getOpt() const
{
	return (this->_opt);
}

std::string const &Server::getServerName() const
{
	return (this->_serverName);
}

std::string const &Server::getPassword() const
{
	return (this->_password);
}

std::map<std::string, Channel *> const &Server::getChannelMap() const
{
	return (this->_channels);
}

void	Server::removeClient(int clientFd)
{
	PRINT("client disconnected: ", RED, "");
	PRINT(clientFd, RED, "\n");
	std::map<int, Client*>::iterator it = this->_clients.find(clientFd);
	if (it != this->_clients.end())
	{
		if (DEBUG)
			std::cout << DBUG RED "Deleting client : " RESET << it->second->getNickname() << std::endl;
		this->controlEpoll(EPOLL_CTL_DEL, clientFd, NULL);
		close(clientFd);
		delete it->second;
		this->_clients.erase(it);
	}
}

bool validdateChannelName(std::string name)
{
	return (name.length() > 0 && name.length() < 200 && name.find_first_of(7, 0) == name.npos && name.find_first_of(' ',0) == name.npos && name.find_first_of(',', 0) && (name[0] == '&' || name[0] == '#'));
}

std::pair<std::map<std::string, Channel *>::iterator, bool>Server::addChannel(std::string name, std::string password)
{
	std::pair<std::map<std::string, Channel *>::iterator, bool> pair;
	if (!validdateChannelName(name))
	{
		std::cerr << "Error : channel name format incorrect" << std::endl;
		return (pair);
	}

	if (name[0] != '&' && name[0] != '#')
		name = "#" + name;
	Channel *newChan = new Channel(name, password);

	pair = this->_channels.insert(std::make_pair(name, newChan));
	std::cout << DBUG GREEN "Added channel: " RESET << name << std::endl;
	std::cout << DBUG GREEN "Current channels: " RESET;
	for (std::map<std::string, Channel *>::const_iterator it = this->_channels.begin(); it != this->_channels.end(); ++it)
		std::cout << it->first << " ";
	std::cout << std::endl;
	return (pair);
}

void Server::removeChannel(const std::string &name)
{
	std::map<std::string, Channel *>::iterator it = this->_channels.find(name);
	if (it != this->_channels.end())
	{
		std::cout << DBUG RED "Deleting channel : " RESET << name << std::endl;
		delete it->second;
		this->_channels.erase(it);
	}
}

std::map<int, Client*> const &Server::getClientmap() const
{
	return (this->_clients);
}

Channel *Server::getChannelByName(std::string const &name) const
{
	std::map<std::string, Channel *>::const_iterator it = this->_channels.find(name);
	if (it != this->_channels.end())
		return (it->second);
	return (NULL);
}
std::ostream &operator<<(std::ostream &o, const Server &obj)
{
	return (o << "Server name: " << obj.getServerName()
			  << " port: " << obj.getPort());
}

std::string const Server::getClientNickname(int clientFd) const
{
	std::map<int, Client*>::const_iterator it = this->_clients.find(clientFd);
	if (it != this->_clients.end())
		return (it->second->getNickname());
	return ("");
}

Client *Server::getClient(const int fd) const
{
	std::map<int, Client*>::const_iterator it = this->_clients.find(fd);
	if (it != this->_clients.end())
		return (it->second);
	return (NULL);
}

Client *Server::getClient(const std::string nick) const
{
        for (std::map<int, Client*>::const_iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
        {
                if (it->second->getNickname() == nick)
                        return (it->second);
        }
        return (NULL);
}

