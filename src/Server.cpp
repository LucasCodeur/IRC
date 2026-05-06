/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lud-adam <lud-adam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 14:50:09 by lud-adam          #+#    #+#             */
/*   Updated: 2026/05/06 15:13:43 by lud-adam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug.hpp"
#include "Server.hpp"
#include "Exceptions.hpp"

#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>

/**
 * @brief set up the server and launch it.
 * @return true if no errors occur.
 */
bool    Server::launcherServer(void)
{
        this->_server_sock = this->createSocket(AF_INET, SOCK_STREAM, DEFAULT);
        this->setSocketOption(this->_server_sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT);
        this->setAddr();
        this->bindSocket();
        this->listenSocket(MAX_WAITING_LIST);
        this->setEpoll(DEFAULT);
        this->controlEpoll(EPOLL_CTL_ADD, this->_server_sock, &this->_ev); 
        this->listenConnexionsEpoll();

        return (true);
}

/**
 * @brief infinite loop that allows receiving and sending data between the different sockets contains in the ready list of epoll.
 * @return
 */
void    Server::listenConnexionsEpoll(void)
{
    char buffer[1024];
    int nfds;
    socklen_t addrlen = sizeof(this->_addr);
    for (;;)
    {
        nfds = this->epollWaitOperation(MAX_EVENTS, TIMEOUT);
        for (int n = 0; n < nfds; ++n)
        {
            if (this->_events[n].data.fd == this->_server_sock)
            {
                Client test;

                this->_clients.insert(std::pair<int, Client>(this->acceptConnexion(&addrlen), test));
                this->setNonBlocking(test.getFd());
                this->_ev.events = EPOLLIN | EPOLLET;
                this->_ev.data.fd = test.getFd();
                this->sendData(test.getFd(), "Hello from server");
                this->controlEpoll(EPOLL_CTL_ADD, test.getFd(), &this->_ev);
                while (receiveData(buffer) > 0)
                    std::cout << buffer << std::endl;
            } /*else {
            do_use_fd(events[n].data.fd);*/
                       // }
       }
    }
}

/**
 * @brief wrapper function of socket(), allows creating a socket.
 * @param domain, integer that allows specifying communication domain in order to choose the protocol.
 * @param type_communication, TCP or UDP.
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
        this->_ev.events = EPOLLIN;
        this->_ev.data.fd = this->_server_sock;
}

/**
 * @brief wrapper function of epoll_ctl(), control inferface for an epoll file descriptor.
 * @param op request that the operation op be performed for the target file descriptor.
 * @param fd file descriptor targeted, in order to apply an operation on it. 
 * @return
 */
void    Server::controlEpoll(int op, int fd, struct epoll_event* event)
{
        if (epoll_ctl(this->_epollfd, op, fd, event) < 0)
            throw controlEpollFailed();
}

/**
 * @brief wrapper function of accept(), allowing it to extracts the first connection request from the queue of pending connections for the listening socket.
 * @param
 * @param
 * @return
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
    int nfds = epoll_wait(this->_epollfd, this->_events, max_events, timeout);
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
 * @param buffer, allowing it to contain the received data.
 * @return
 */
int    Server::receiveData(char* buffer)
{
    int read = recv(this->_client_sock, buffer, sizeof(buffer), 0);
    if (read > 0)
        throw receiveDataFailed(); 
    return (read);
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

Server::Server (void)
{
    std::cout << "Server constructor called" << std::endl;
    this->_opt = 1;
}

Server::~Server (void)
{
    std::cout << "Server deconstructor called" << std::endl;
}
