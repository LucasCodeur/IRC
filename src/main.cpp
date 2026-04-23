/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lud-adam <lud-adam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 11:38:11 by lud-adam          #+#    #+#             */
/*   Updated: 2026/04/13 11:40:04 by lud-adam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdarg.h>
#include <sys/epoll.h>

# define MAX_EVENTS 10
# define BUF_SIZE 200
# define SERVER_PORT 6667

int	main(void)
{

	int listen_sock, conn_sock, opt;
	int	read_bytes;
	struct sockaddr_in serv_addr;
	char buf[BUF_SIZE];
	int err, nfds;

	struct epoll_event ev, events[MAX_EVENTS];
	int epollfd;
	if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	dprintf(2, "socket error\n");

	bzero(&serv_addr, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(SERVER_PORT);

	opt = 1;
	if (setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
	{
		close(listen_sock);
		perror("setsockopt error");
		return (1);
	}

	if ((err = bind(listen_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr))) < 0)
	{
		perror("bind error");
		return (err);
	}

	if (listen(listen_sock, 10) < 0)
	{
		perror("listen error\n");
		return (1);
	}

	bzero(buf, BUF_SIZE);
	printf("Waiting for connection on port %d\n", SERVER_PORT);


	epollfd = epoll_create1(0);
	if (epollfd == -1)
	{
		perror("epoll_create1");
		exit(EXIT_FAILURE);
	}

	ev.events = EPOLLIN;
	ev.data.fd = listen_sock;
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_sock, &ev) == -1)
	{
		perror("epoll_ctl: listen_sock");
		exit(EXIT_FAILURE);
	}

	for (;;)
	{
		nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
		if (nfds == -1)
		{
			perror("epoll_wait");
			exit(EXIT_FAILURE);
		}

		for (int n = 0; n < nfds; ++n)
		{
			if (events[n].data.fd == listen_sock)
			{
				conn_sock = accept(listen_sock, (struct sockaddr *) NULL, NULL);
				if (conn_sock == -1)
				{
					perror("accept");
					exit(EXIT_FAILURE);
				}
			// setnonblocking(conn_sock);
				ev.events = EPOLLIN | EPOLLET;
				ev.data.fd = conn_sock;
				if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock, &ev) == -1)
				{
					perror("epoll_ctl: conn_sock");
					exit(EXIT_FAILURE);
				}
			}
			else
			{
				while ((read_bytes = read(conn_sock, buf, BUF_SIZE)))
				{
					buf[read_bytes - 1] = '\0';
					printf("%d sent : %s\n", conn_sock, buf);
				}
			}
		}
	}

	return (0);
}
