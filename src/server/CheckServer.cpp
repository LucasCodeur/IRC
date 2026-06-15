#include "Server.hpp"

#include <cctype>
#include <sstream>
#include <stdexcept>

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
void    Server::check_password(std::string& password)
{
    int size = password.size();
    for (int i = 0; i < size; i++) 
    {
        if (std::isspace(password[i]))
            throw std::runtime_error("Bad password");
    }
}
