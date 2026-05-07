/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lud-adam <lud-adam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 14:54:55 by lud-adam          #+#    #+#             */
/*   Updated: 2026/05/06 17:20:57 by lud-adam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# define PORT 8080
# define MAX_EVENTS 30
# define MAX_WAITING_LIST 3
# define TIMEOUT -1 
# define DEFAULT 0 
# define BUFFER_SIZE 1024

#include "Client.hpp"
#include "Channel.hpp"
#include "Command.hpp"

#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <map>

#include <ostream>
#include <vector>


class Server
{
    public :
        Server(void);
	Server(Server const &original);
	Server &operator=(Server const &other);
        ~Server();
        bool    launcherServer(void);

	// GETTERS
	std::string const &getServerName() const;
	std::string const &getPassword() const;
	int getPort() const;
	int getFd() const;
	void                    handleJoin(Command const &cmd);
	void                    handleCommand(Command const &cmd);
    private :
        int					_port;
        int                                     _fd;
        int                                     _server_sock;
        std::string				_serverName;
        std::string				_password;
        std::map<int, Client>	                _clients;
        std::map<std::string, Channel>	        _channels;
        struct sockaddr_in      _addr; // contains the IP adress and port number to bind the socket.
        struct epoll_event      _ev[MAX_EVENTS];
        int                     _epollfd;
        int                     _opt;
        int                     createSocket(int domain, int type_communication, int protocol);
        int                     acceptConnexion(socklen_t* addrlen);
        int                     epollWaitOperation(int max_events, int timeout);
        int                     receiveData(int socketfd, char* buffer);
        void                    setSocketOption(int socket_fd, int level, int option_name);
        void                    bindSocket(void);
        void                    listenSocket(int sizeWaitingList);
        void                    setAddr(void);
        void                    setEpoll(int option);
        void                    controlEpoll(int op, int fd, struct epoll_event* event);
        void                    listenConnexionsEpoll(void);
        void                    sendData(int fd, std::string data);
        void                    setNonBlocking(int sock);
	private:
};
std::ostream &operator<<(std::ostream &o, const Server &obj);

#endif
