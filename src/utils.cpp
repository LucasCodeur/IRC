/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lud-adam <lud-adam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/21 18:04:43 by lud-adam          #+#    #+#             */
/*   Updated: 2026/06/21 18:22:03 by lud-adam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <ostream>
#include <sys/socket.h>
#include <string.h>
#include <sstream>

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
	char	buffer[BUFFER_SIZE] = {"0"};

	memset(buffer, 0, BUFFER_SIZE);
	bytes_read = recv(socket, buffer, sizeof(buffer), 0);
	if (bytes_read <= 0)
	{
		if (bytes_read == 0 || ((bytes_read == -1) && (errno != EAGAIN && errno != EWOULDBLOCK)))
			return (false);
	}
	buffer[bytes_read] = '\0';
	buf += buffer;
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

}
