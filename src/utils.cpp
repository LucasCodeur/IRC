/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enchevri <enchevri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/21 18:04:43 by lud-adam          #+#    #+#             */
/*   Updated: 2026/07/10 20:24:34 by enchevri         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdexcept>
#include <sstream>

#include <sys/socket.h>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>

# define BUFFER_SIZE 2048

namespace utils_server 
{
/**
 * @brief wrapper function of recv(), allowing it to receive data by the indicated file descriptor.
 * @param socketfd to receive data from this one.
 * @param buf .
 * @return true if no problem or false if a there is a problem.
 */
bool	receiveData(int socket, std::string& buf)
{
	int	bytes_read;
	char	buffer[BUFFER_SIZE];

	bytes_read = recv(socket, buffer, sizeof(buffer), 0);
	if (bytes_read <= 0)
	{
		return (false);
	}
	buf.append(buffer, bytes_read);
	return (true);
}

static bool check_port(std::string& port);

/**
 * @brief function to check if the port is correct and convert this one.
 * @param port string to convert into number.
 * @return true if the port is correct or false if not the case.
 */
bool    convertPort(std::string port, int& portToSet)
{
    if (check_port(port) == false)
        throw std::runtime_error("Bad characters inside port");

    std::stringstream ss(port);
    if (ss.fail() == true)
        throw std::runtime_error("Bad characters inside port");

    ss >> portToSet;
    if (1023 >= portToSet || portToSet >= 49152)
        throw std::runtime_error("Bad range of port");

    return (true);
}

/**
 * @brief function to check if only digit inside port.
 * @param port string to check.
 * @return true if correct, false or not.
 */
static bool check_port(std::string& port)
{
    int size = port.size();
    for (int i = 0; i < size; i++)
    {
        if (std::isdigit(port[i]) == false)
            return (false);
    }
    return (true);
}

/**
* @brief function to check if password is correct.
* @param password string to check.
* @return
*/
void	check_password(std::string& password)
{
    int size = password.size();
    for (int i = 0; i < size; i++) 
    {
        if (std::isspace(password[i]))
            throw std::runtime_error("Bad password");
    }
}

/**
 * @brief function to extract a valid command from the buffer.
 * @param buffer, string to extract the command.
 * @return a valid command.
 */
std::string extractCommand(std::string& buffer, bool security)
{
	std::string		res;
	size_t			pos = buffer.find("\r");

	if (pos != std::string::npos)
	{
		res = buffer.substr(0, pos + 1);
		buffer.erase(0, pos + 2);
	}
	(void)security;
	int size = res.size();
	res = res.substr(0, size - 1);
	return (res);
}

/**
* @brief wrapper function to fcntl(), allowing to set up the socket in a non blocking-mode.
* @return
*/
void	setNonBlocking(int sock)
{
	int result;

	result = fcntl(sock , F_SETFL , O_NONBLOCK);
	if (result == -1)
		throw std::runtime_error("fcntl failed");
}

/**
 * @brief wrapper function of send(), allowing it to send data by the indicated socket.
 * @param data, data to send.
 * @param fd file descriptor where data will be sent.
 * @return
 */
void	sendData(int fd, std::string &data)
{
	ssize_t sent = send(fd, data.c_str(), data.size(), 0);
	if (sent > 0)
		data.erase(0, sent);
	else if (sent == -1)
		throw std::runtime_error("Send failed");
}

/**
 * @brief function to set up the behavior of the socket.
 * @return
 */
void	setAddr(struct sockaddr_in& addr, int port)
{
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);
}

}
