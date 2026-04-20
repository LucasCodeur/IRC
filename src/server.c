/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lud-adam <lud-adam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 14:50:09 by lud-adam          #+#    #+#             */
/*   Updated: 2026/04/20 15:42:06 by lud-adam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug.hpp"

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

static int setnonblocking(int sock);

bool    server(void)
{
        struct epoll_event ev, events[MAX_EVENTS];
        int listen_sock, conn_sock, nfds, epollfd;
        int opt = 1;

        struct sockaddr_in addr;

        socklen_t addrlen = sizeof(addr);
        char buffer[1024] = { 0 };
        ssize_t valread;
        std::string hello = "Hello from server";

        if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            perror("socked failed");
            exit(EXIT_FAILURE);
        }

        if (setsockopt(listen_sock, SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
        {
                perror("setsockopt");
                exit(EXIT_FAILURE);
        }

        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(PORT);

        if (bind(listen_sock, (struct sockaddr*)&addr,
                 sizeof(addr))
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

        ev.events = EPOLLIN;
        ev.data.fd = listen_sock;
        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_sock, &ev) == -1) {
           perror("epoll_ctl: listen_sock");
           exit(EXIT_FAILURE);
        }

        int new_socket = 0;

        for (;;) {
           nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
           if (nfds == -1) {
               perror("epoll_wait");
               exit(EXIT_FAILURE);
           }

           for (int n = 0; n < nfds; ++n) {
               if (events[n].data.fd == listen_sock) {
                   conn_sock = accept(listen_sock,
                                      (struct sockaddr *) &addr, &addrlen);
                   if (conn_sock == -1) {
                       perror("accept");
                       exit(EXIT_FAILURE);
                   }

                    valread = read(listen_sock, buffer,
                                   1024 - 1); 
                    printf("%s\n", buffer);
                    send(new_socket, hello.c_str(), strlen(hello.c_str()), 0);
                    printf("Hello message sent\n");
                   setnonblocking(conn_sock);
                   ev.events = EPOLLIN | EPOLLET;
                   ev.data.fd = conn_sock;
                   if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock,
                               &ev) == -1) {
                       perror("epoll_ctl: conn_sock");
                       exit(EXIT_FAILURE);
                   }
               } /*else {
                   do_use_fd(events[n].data.fd);*/
               // }
           }
        }
        close(epollfd);
        close(new_socket);
}

static int setnonblocking(int sock)
{
    int result;
    int flags;

    flags = ::fcntl(sock, F_GETFL, 0);

    if (flags == -1)
    {
        return -1;  // error
    }

    flags |= O_NONBLOCK;

    result = fcntl(sock , F_SETFL , flags);
    return result;
}
