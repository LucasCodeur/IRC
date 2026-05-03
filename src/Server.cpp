/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lud-adam <lud-adam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 14:50:09 by lud-adam          #+#    #+#             */
/*   Updated: 2026/05/03 15:09:00 by lud-adam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug.hpp"
#include "Server.hpp"
#include "Exceptions.hpp"

#include <sys/socket.h>
#include <sys/epoll.h>
#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>

# define MAX_EVENTS 10
# define PORT 8080

Server::Server (void)
{
    std::cout << "Server constructor called" << std::endl;
    this->opt = 1;
}

Server::~Server (void)
{
    std::cout << "Server deconstructor called" << std::endl;
}

int    Server::createSocket(int domain, int type_communication, int protocol)
{
    int sockfd = socket(domain, type_communication, protocol);
    if (sockfd < 0)
        throw socketFailed();
    return (sockfd);
}

void     Server::setSocketOption(int socketfd, int level, int option_name)
{
    int ret = setsockopt(socketfd, level, option_name, &this->opt, sizeof(this->opt));
    if (ret < 0)
        throw setSocketOptionFailed();
}

static int setnonblocking(int sock);

bool    Server::setServer(void)
{
        int listen_sock, conn_sock, nfds, epollfd;

        socklen_t addrlen = sizeof(this->addr);
        char buffer[1024] = { 0 };
        std::string hello = "Hello from server";

        listen_sock = this->createSocket(AF_INET, SOCK_STREAM, 0);
        this->setSocketOption(listen_sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT);
        this->addr.sin_family = AF_INET;
        this->addr.sin_addr.s_addr = INADDR_ANY;
        this->addr.sin_port = htons(PORT);

        if (bind(listen_sock, (struct sockaddr*)&this->addr,
                 sizeof(this->addr))
            < 0) {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }

        if (listen(listen_sock, 3) < 0) {
            perror("listen");
            exit(EXIT_FAILURE);
        }

        epollfd = epoll_create1(0);
        if (epollfd == -1) {
           perror("epoll_create1");
           exit(EXIT_FAILURE);
        }

        this->ev.events = EPOLLIN;
        this->ev.data.fd = listen_sock;
        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_sock, &this->ev) == -1) {
           perror("epoll_ctl: listen_sock");
           exit(EXIT_FAILURE);
        }

        for (;;)
        {
           nfds = epoll_wait(epollfd, this->events, MAX_EVENTS, -1);
            if (nfds == -1)
            {
               perror("epoll_wait");
               exit(EXIT_FAILURE);
            }

           for (int n = 0; n < nfds; ++n)
            {
               if (this->events[n].data.fd == listen_sock)
                {
                   conn_sock = accept(listen_sock,
                                      (struct sockaddr *) &this->addr, &addrlen);
                   if (conn_sock == -1)
                    {
                       perror("accept");
                       exit(EXIT_FAILURE);
                   }
                    send(conn_sock, hello.c_str(), strlen(hello.c_str()), 0);

                   setnonblocking(conn_sock);
                   this->ev.events = EPOLLIN | EPOLLET;
                   this->ev.data.fd = conn_sock;
                   if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock,
                               &this->ev) == -1) {
                       perror("epoll_ctl: conn_sock");
                       exit(EXIT_FAILURE);
                   }

                    while (recv(conn_sock, &buffer, sizeof(buffer), 0) > 0)
                        printf("%s\n", buffer);
               } /*else {
                   do_use_fd(events[n].data.fd);*/
               // }
           }
            printf("for the less imbricate\n");
        }
}

static int setnonblocking(int sock)
{
    int result;
    int flags;

    flags = ::fcntl(sock, F_GETFL, 0);
    if (flags == -1)
        return -1;
    flags |= O_NONBLOCK;
    result = fcntl(sock , F_SETFL , flags);
    return (result);
}
