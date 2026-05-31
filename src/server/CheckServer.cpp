/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CheckServer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enchevri <enchevri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 09:58:12 by lud-adam          #+#    #+#             */
/*   Updated: 2026/05/31 17:17:30 by enchevri         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Exceptions.hpp"
#include <cctype>
#include <sstream>

static bool check_port(std::string& port);

/**
 * @brief method to check if the port is correct and convert this one.
 * @param port string to convert into number.
 * @return true if the port is correct or false if not the case.
 */
bool    Server::convertPort(std::string port)
{
    if (check_port(port) == false)
        throw badCharactersInsidePort();

    std::stringstream ss(port);
    if (ss.fail() == true)
        throw badCharactersInsidePort();

    ss >> this->_port;
    if (1023 >= this->_port || this->_port >= 49152)
        throw badRangePort();

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
void    Server::check_password(std::string& password)
{
    int size = password.size();
    for (int i = 0; i < size; i++) 
    {
        if (std::isspace(password[i]))
            throw passwordNotCorrect();
    }
}
