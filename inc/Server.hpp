/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lud-adam <lud-adam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 14:54:55 by lud-adam          #+#    #+#             */
/*   Updated: 2026/05/03 15:07:58 by lud-adam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# define PORT 8080
# define MAX_EVENTS 10
# define MAX_WAITING_LIST 3
# define TIMEOUT -1 
# define DEFAULT 0 

#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>

bool    server(void);

class Server
{
    public :
        Server(void);
        ~Server();
        bool    setServer(void);
        int     createSocket(int domain, int type_communication, int protocol);
        void    setSocketOption(int socket_fd, int level, int option_name);
        void    bindSocket(void);
        void    listenSocket(int sizeWaitingList);
        void    setAddr(void);
        void    setEpoll(int option);
        void    controlEpoll(int op, int fd, struct epoll_event* event);
        void    listenConnexionsEpoll(void);
        int     acceptConnexion(socklen_t* addrlen);
        int     epollWaitOperation(void);
        void    sendData();
        int     receiveData(char* buffer);

        struct sockaddr_in  addr;
        struct epoll_event  events[MAX_EVENTS];
        struct epoll_event  ev;
        int server_sock;
        int client_sock;
        int opt;

    private :
        int epollfd;
};

#endif
