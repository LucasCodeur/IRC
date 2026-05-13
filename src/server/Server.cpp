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
#include <stdio.h>
#include <errno.h>

#include <iostream>
#include <string>

/**
 * @brief set up the server and launch it.
 * @return true if no errors occur.
 */
bool    Server::launcherServer(std::string port, std::string password)
{
    try 
    {
        this->convertPort(port);
        this->check_password(password);
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
void    Server::listenConnexionsEpoll(void)
{
    socklen_t addrlen = sizeof(this->_addr);
    int nfds = 1;

    while (true) 
    {
        nfds = this->epollWaitOperation(MAX_EVENTS, TIMEOUT);
        for (int n = 0; n < nfds; n++)
        {
            int fd;
            if (this->_ev[n].data.fd == this->_server_sock)
            {
                fd = this->acceptConnexion(&addrlen);
                this->setNonBlocking(fd);
                this->_ev[n + 1].events = EPOLLIN | EPOLLET;
                this->_ev[n + 1].data.fd = fd;
                this->sendData(fd, "Welcome to the IRC SERVER");
                this->controlEpoll(EPOLL_CTL_ADD, fd, &this->_ev[n + 1]);
                PRINT("Client connected: ", GREEN, "");
                PRINT(fd, WHITE, "\n");
            } 
            else if (this->_ev[n].events & EPOLLIN)
                this->receiveData(this->_ev[n].data.fd);
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
int    Server::createSocket(int domain, int type_communication, int protocol)
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
void     Server::setSocketOption(int socket_fd, int level, int option_name)
{
    if (setsockopt(socket_fd, level, option_name, &this->_opt, sizeof(this->_opt)) < 0)
        throw setSocketOptionFailed();
}

/**
 * @brief wrapper function of bind(), allows binding the socket to the adress and port number specified in addr.
 * @return
 */
void    Server::bindSocket(void)
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
void    Server::listenSocket(int sizeWaitingList)
{
        if (listen(this->_server_sock, sizeWaitingList) < 0)
            throw listenSocketFailed();
}

/**
 * @brief wrapper function of epoll_create1(), allows the "watching" of multiple file descriptor.
 * @param option allow to get different behavior, 0 is default.
 * @return
 */
void    Server::setEpoll(int option)
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
void    Server::controlEpoll(int op, int fd, struct epoll_event* event)
{
        if (epoll_ctl(this->_epollfd, op, fd, event) < 0)
            throw controlEpollFailed();
}

/**
 * @brief wrapper function of accept(), allowing it to extracts the first connection request from the queue of pending connections for the listening socket.
 * @param addrlen  pointer to a variable that specifies the length of the adress structure.
 * @return new file descriptor referring to the first connection request from the queue of pending connections for the listening socket.
 */
int    Server::acceptConnexion(socklen_t* addrlen)
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
int    Server::epollWaitOperation(int max_events, int timeout)
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
void    Server::sendData(int fd, std::string data)
{
    if (send(fd, data.c_str(), strlen(data.c_str()), 0) < 0)
        throw sendFailed();
}

/**
 * @brief wrapper function of recv(), allowing it to receive data by the indicated socket.
 * @param socketfd to receive data from this one.
 * @return
 */
void    Server::receiveData(int socketfd)
{
    Client temp;

    temp.setFd(socketfd);
    this->_clients.insert(std::pair<int, Client>(socketfd, temp));
    int bytes_read;
    char buffer[BUFFER_SIZE] = {"0"};

    while (1)
    {
            bytes_read = recv(socketfd, buffer, sizeof(buffer), 0);
            // PRINT("Bytes_read: ", BLUE, "");
            // PRINT(bytes_read, WHITE, "\n");
            if (bytes_read <= 0)
                break ;
            PRINT("received: ", GREEN, "");
            PRINT(socketfd, GREEN, "\n");
            PRINT(buffer, GREEN, "\n");
    }
    if (bytes_read <= 0)
    {
        if (bytes_read == 0 || (bytes_read == -1 && (errno != EAGAIN && errno != EWOULDBLOCK)))
        {
            PRINT("client disconnected: ", RED, "");
            PRINT(socketfd, RED, "\n");
            close(socketfd);
            this->controlEpoll(EPOLL_CTL_DEL, socketfd, NULL);
        }
    }
}

/**
 * @brief function to set up the behavior of the socket.
 * @return
 */
void    Server::setAddr(void)
{
        this->_addr.sin_family = AF_INET;
        this->_addr.sin_addr.s_addr = INADDR_ANY;
        this->_addr.sin_port = htons(PORT);
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
        throw setnonblockingFailed();
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

Server::Server(int port, const std::string &password)
	: _port(port),
	  _fd(-1),
	  _serverName("ircserv"),
	  _password(password),
          _opt(1)
{
	if (DEBUG == 1)
		std::cout << DBUG GREEN "Server created: " RESET << *this << std::endl;
} 

Server::~Server()
{
	for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
		delete it->second;
	if (DEBUG == 1)
		std::cout << DBUG RED "Server destroyed: " RESET << *this << std::endl;
}

Server::Server(Server const &original)
	: _port(original._port),
	  _fd(original._fd),
	  _serverName(original._serverName),
	  _password(original._password),
	  _clients(original._clients),
          _opt(original._opt)
{
	if (DEBUG == 1)
		std::cout << DBUG BLUE "Server copied: " RESET << *this << std::endl;
}

Server &Server::operator=(Server const &other)
{
	if (this != &other)
	{
		this->_opt = other._opt;
		this->_port = other._port;
		this->_fd = other._fd;
		this->_serverName = other._serverName;
		this->_password = other._password;
		this->_clients = other._clients;
		if (DEBUG == 1)
			std::cout << DBUG BLUE "Server assigned: " RESET << *this << std::endl;
	}
	return (*this);
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

std::pair<std::map<std::string, Channel *>::iterator, bool>Server::addChannel(std::string name, std::string password)
{
        Channel *newChan = new Channel(name, password);

        std::pair<std::map<std::string, Channel *>::iterator, bool> pair;
        pair = this->_channels.insert(std::make_pair(name, newChan));
        std::cout << DBUG GREEN "Created channel : " RESET << name << std::endl;
        return (pair);
}

std::map<int, Client> const &Server::getClientmap() const
{
        return (this->_clients);
}

std::ostream &operator<<(std::ostream &o, const Server &obj)
{
	return (o << "Server name: " << obj.getServerName()
			  << " port: " << obj.getPort());
}
