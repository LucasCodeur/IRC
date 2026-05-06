/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lud-adam <lud-adam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 14:54:55 by lud-adam          #+#    #+#             */
/*   Updated: 2026/05/06 11:28:54 by lud-adam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# define PORT 8080
# define MAX_EVENTS 30
# define MAX_WAITING_LIST 3
# define TIMEOUT -1 
# define DEFAULT 0 

#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <map>
#include <ostream>
#include <string>
#include "Channel.hpp"
#include "Command.hpp"
#include "Client.hpp"

#include <ostream>

class Server
{
    public :
        Server(void);
        ~Server();
        bool    launcherServer(void);
        int     createSocket(int domain, int type_communication, int protocol);
        void    setSocketOption(int socket_fd, int level, int option_name);
        void    bindSocket(void);
        void    listenSocket(int sizeWaitingList);
        void    setAddr(void);
        void    setEpoll(int option);
        void    controlEpoll(int op, int fd, struct epoll_event* event);
        void    listenConnexionsEpoll(void);
        int     acceptConnexion(socklen_t* addrlen);
        int     epollWaitOperation(int max_events, int timeout);
        void    sendData(std::string data);
        int     receiveData(char* buffer);
        void    setNonBlocking(int sock);

        struct sockaddr_in  addr; // contains the IP adress and port number to bind the socket.
        struct epoll_event  events[MAX_EVENTS];
        struct epoll_event  ev;
        int server_sock;
        int client_sock;
        int _opt;

	// CONSTRUCTOR
	Server(int port, const std::string &password);

	// GETTERS
	int getPort() const;
	int getFd() const;
        int getOpt() const;
	std::string const &getServerName() const;
	std::string const &getPassword() const;
	std::map<int, Client> const &getClientmap() const;
	std::map<std::string, Channel*> const &getChanelmap() const;
	
	// METHODS
	void handleCommand(Command const &cmd);

    private :
		int								_port;
		int								_fd;
		std::string						_serverName;
		std::string						_password;
		std::map<int, Client>			_clients;
		Server(Server const &original);
		Server &operator=(Server const &other);
		void handleJoin(Command const &cmd);
		std::map<std::string, Channel*>	_channels;
		int epollfd;
};
std::ostream &operator<<(std::ostream &o, const Server &obj);

#endif
