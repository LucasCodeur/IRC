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

bool    Server::setServer(void)
{
        this->server_sock = this->createSocket(AF_INET, SOCK_STREAM, DEFAULT);
        this->setSocketOption(this->server_sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT);
        this->setAddr();
        this->bindSocket();
        this->listenSocket(MAX_WAITING_LIST);
        this->setEpoll(DEFAULT);
        this->controlEpoll(EPOLL_CTL_ADD, this->server_sock, &this->ev); 
        this->listenConnexionsEpoll();

        return (true);
}

int    Server::createSocket(int domain, int type_communication, int protocol)
{
    int sockfd = socket(domain, type_communication, protocol);
    if (sockfd < 0)
        throw socketFailed();
    return (sockfd);
}

void     Server::setSocketOption(int socket_fd, int level, int option_name)
{
    if (setsockopt(socket_fd, level, option_name, &this->opt, sizeof(this->opt)) < 0)
        throw setSocketOptionFailed();
}

void    Server::bindSocket(void)
{
        if (bind(this->server_sock, (struct sockaddr*)&this->addr, sizeof(this->addr)) < 0)
            throw bindFailed();
}

void    Server::listenSocket(int sizeWaitingList)
{
        if (listen(this->server_sock, sizeWaitingList) < 0)
            throw listenSocketFailed();
}


void    Server::setEpoll(int option)
{
        this->epollfd = epoll_create1(option);
        if (epollfd == -1)
            throw epollCreateFailed();
        this->ev.events = EPOLLIN;
        this->ev.data.fd = this->server_sock;
}

void    Server::controlEpoll(int op, int fd, struct epoll_event* event)
{
        if (epoll_ctl(this->epollfd, op, fd, event) < 0)
            throw controlEpollFailed();
}

int    Server::acceptConnexion(socklen_t* addrlen)
{
    int fd = accept(this->server_sock, (struct sockaddr *)&this->addr, addrlen);
    if (fd < 0)
        throw acceptFailed(); 
    return (fd);
}

int    Server::epollWaitOperation(void)
{
    int fd = epoll_wait(this->epollfd, this->events, MAX_EVENTS, TIMEOUT);
    if (fd < 0)
        throw epollWaitFailed();
    return (fd);
}

void    Server::sendData()
{
    std::string hello = "Hello from server";
    if (send(client_sock, hello.c_str(), strlen(hello.c_str()), 0) < 0)
        throw sendFailed();
}

int    Server::receiveData(char* buffer)
{
    int read = recv(client_sock, buffer, sizeof(buffer), 0);
    if (read > 0)
        throw receiveDataFailed(); 
    return (read);
}

void    Server::setAddr(void)
{
        this->addr.sin_family = AF_INET;
        this->addr.sin_addr.s_addr = INADDR_ANY;
        this->addr.sin_port = htons(PORT);
}

static int setnonblocking(int sock);

void    Server::listenConnexionsEpoll(void)
{
    char buffer[1024];
    int nfds;
    socklen_t addrlen = sizeof(this->addr);
    for (;;)
    {
        nfds = this->epollWaitOperation();
        for (int n = 0; n < nfds; ++n)
        {
           if (this->events[n].data.fd == this->server_sock)
            {
               this->client_sock = this->acceptConnexion(&addrlen);
               setnonblocking(this->client_sock);
               this->ev.events = EPOLLIN | EPOLLET;
               this->ev.data.fd = client_sock;
                this->sendData();
                this->controlEpoll(EPOLL_CTL_ADD, this->client_sock, &this->ev);
                while (receiveData(buffer) > 0)
                    printf("%s\n", buffer);
           } /*else {
               do_use_fd(events[n].data.fd);*/
           // }
       }
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

Server::Server (void)
{
    std::cout << "Server constructor called" << std::endl;
    this->opt = 1;
}

Server::~Server (void)
{
    std::cout << "Server deconstructor called" << std::endl;
}
