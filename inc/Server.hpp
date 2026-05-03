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
        void    setSocketOption(int socketfd, int level, int option_name);
        struct sockaddr_in  addr;
        struct epoll_event  events[MAX_EVENTS];
        struct epoll_event  ev;
        int server_sock;
        int client_sock;
        int nfds;
        int epollfd;
        int opt;
        char buffer[1024];

    private :
};

#endif
