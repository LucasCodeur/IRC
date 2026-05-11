/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lud-adam <lud-adam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 14:54:55 by lud-adam          #+#    #+#             */
/*   Updated: 2026/05/07 16:13:16 by kbarru           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# define PORT 8080
# define MAX_EVENTS 30
# define MAX_WAITING_LIST 3
# define TIMEOUT -1 
# define DEFAULT 0 
# define BUFFER_SIZE 4096 

#include "Client.hpp"
#include "Channel.hpp"
#include "Command.hpp"

#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <map>

#include <map>
#include <ostream>
#include <string>
#include "Channel.hpp"
#include "Client.hpp"

#include <ostream>
#include <vector>

class Server
{
    public :
        Server(void);
	Server(Server const &original);
        Server(int port, const std::string &password);
	Server &operator=(Server const &other);
        ~Server();
        bool    launcherServer(void);
	int getPort() const;
	int getFd() const;
        int getOpt() const;
	std::string const &getServerName() const;
	std::string const &getPassword() const;
	std::map<int, Client> const &getClientmap() const;
	
	// METHODS
        std::pair<std::map<std::string, Channel *>::iterator, bool> addChannel(std::string s, std::string password);
	void                    handleJoin(Command const &cmd);
	void                    handleCommand(Command const &cmd);
	std::map<std::string, Channel*> const &getChannelMap() const;
    private :
        int					_port;
        int                                     _fd;
        int                                     _server_sock;
        std::string				_serverName;
        std::string				_password;
        std::map<int, Client>	                _clients;
        std::map<std::string, Channel*>	        _channels;
        struct sockaddr_in      _addr; // contains the IP adress and port number to bind the socket.
        struct epoll_event      _ev[MAX_EVENTS];
        int                     _epollfd;
        int                     _opt;
        int                     createSocket(int domain, int type_communication, int protocol);
        int                     acceptConnexion(socklen_t* addrlen);
        int                     epollWaitOperation(int max_events, int timeout);
        void                    receiveData(int socketfd);
        void                    setSocketOption(int socket_fd, int level, int option_name);
        void                    bindSocket(void);
        void                    listenSocket(int sizeWaitingList);
        void                    setAddr(void);
        void                    setEpoll(int option);
        void                    controlEpoll(int op, int fd, struct epoll_event* event);
        void                    listenConnexionsEpoll(void);
        void                    sendData(int fd, std::string data);
        void                    setNonBlocking(int sock);
};
std::ostream &operator<<(std::ostream &o, const Server &obj);

#endif
