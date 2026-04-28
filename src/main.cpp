/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lud-adam <lud-adam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 11:38:11 by lud-adam          #+#    #+#             */
/*   Updated: 2026/04/28 10:19:18 by kbarru           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <cstdlib>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>

# define MAX_EVENTS 3
# define BUF_SIZE 200
# define SERVER_PORT 6667

int	main(void)
{
	int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	int client_sock;
	struct sockaddr_in	listening_addr;
	if (listen_sock < 0)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}

	int opt = 1;
	setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)); // set reuse address
	
	memset(&listening_addr, 0, sizeof(listening_addr));
	listening_addr.sin_family = AF_INET;
	listening_addr.sin_addr.s_addr = INADDR_ANY;
	listening_addr.sin_port = htons(SERVER_PORT);

	if (bind(listen_sock, (struct sockaddr *)&listening_addr, sizeof(listening_addr)))
	{
		perror("bind");
		exit(EXIT_FAILURE);
	}
	
	if (listen(listen_sock, MAX_EVENTS))
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

	printf("waiting for connections...\n");

	int nfds = 0;
	int sock_count = 1;
	struct pollfd fds[MAX_EVENTS];
	memset(fds, 0, sizeof(fds));
	fds[0].fd = listen_sock;
	fds[0].events = POLLIN;

	while (true)
	{
		nfds = poll(fds, sock_count, -1);
		if (nfds < 0)
		{
			perror("poll");
			exit(EXIT_FAILURE);
		}

		for (int i = 0; i < sock_count; ++i)
		{
			if (fds[i].revents & POLLIN)
			{
				if (fds[i].fd == listen_sock)
				{
					client_sock = accept(fds[i].fd, NULL, NULL);
					if (sock_count == MAX_EVENTS)
					{
						printf("refusing connection : max client count reached\n");
						close(client_sock);
						continue ;
					}

					if (client_sock < 0)
					{
						perror("accept");
						exit(EXIT_FAILURE);
					}
					fds[sock_count].fd = client_sock;
					fds[sock_count].events = POLLIN;
					++sock_count;
					printf("[%d]	Client connected\n", client_sock);
				}
				else
				{
					int		bytes_read = 0;
					char	buffer[BUF_SIZE + 1];
					memset(buffer, 0, BUF_SIZE + 1);
					bytes_read = recv(fds[i].fd, buffer, BUF_SIZE, 0);
					if (bytes_read <= 0)
					{
						if (bytes_read < 0)
							perror("recv");
						else
							printf("[%d]	client disconnected\n", fds[i].fd);
						close(fds[i].fd);
						if (i < sock_count)
							fds[i] = fds[sock_count - 1];
						fds[sock_count - 1].fd = -1;
						--sock_count;
					}
					else
					{
						buffer[bytes_read] = 0;
						printf("[%d]	received %s\n", fds[i].fd, buffer);
					}
				}
			}
		}
	}
	return (0);
}
