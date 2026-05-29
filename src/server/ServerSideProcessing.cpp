/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSideProcessing.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lud-adam <lud-adam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 14:30:46 by lud-adam          #+#    #+#             */
/*   Updated: 2026/05/27 10:44:22 by lud-adam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Exceptions.hpp"
#include "Server.hpp"
#include "Command.hpp"
#include "debug.hpp"
#include "CommandFactory.hpp"

#include <exception>
#include <stdexcept>
#include <stdio.h>
#include <errno.h>
#include <bits/stdc++.h>

static std::string    extractCommand(std::string& buffer);

/**
 * @brief wrapper function of recv(), allowing it to receive data by the indicated file descriptor.
 * @param socketfd to receive data from this one.
 * @return
 */
void    Server::receiveData(int clientFd)
{
    std::map<int, Client*>::const_iterator it = this->_clients.find(clientFd);
    if (it == this->_clients.end())
    {
        Client* temp = new Client; 
        temp->setFd(clientFd);
        this->_clients.insert(std::pair<int, Client*>(clientFd, temp));
    }
    int bytes_read;
    char buffer[BUFFER_SIZE] = {"0"};
    std::string stringBuf;
    std::string strCommand;

    while (1)
    {
        Command* command;
        bytes_read = recv(clientFd, buffer, sizeof(buffer), 0);
        buffer[bytes_read] = '\0';
        PRINT("command received from client ", GREEN, "");
        PRINT(clientFd, GREEN, "");
        PRINT("\n", GREEN, "");
        try
        {
            stringBuf += buffer;
            // std::cout << "stringBuf: " << stringBuf << std::endl;
            strCommand = extractCommand(stringBuf);
            memset(buffer, 0, BUFFER_SIZE);
            command = CommandFactory::createCommand(this, clientFd, strCommand);
            command->execute();
        }
        catch(Command::UnknownCommandException& e)
        {
            Client *originClient = this->getClientByFd(clientFd);
            std::cout << "Caught: " << e.what() << std::endl;
            std::string cmdKeyword = e.what();
            std::string reply = command->getDirector()->errUnknownCommand(*originClient, cmdKeyword);
            this->sendData(clientFd, reply);
            continue ;
        }
        catch(std::exception& e)
        {
            std::cout << "Caught: " << e.what() << std::endl;
            return ; //FIXME: Maybe take off the client instead.
        }
        if (bytes_read <= 0)
        {
            if (bytes_read == 0 || (bytes_read == -1 && (errno != EAGAIN && errno != EWOULDBLOCK)))
            {
                PRINT("client disconnected: ", RED, "");
                PRINT(clientFd, RED, "\n");
                close(clientFd);
                this->controlEpoll(EPOLL_CTL_DEL, clientFd, NULL);
            }
        }
        // std::map<int, Client*>::const_iterator it = this->_clients.find(clientFd);
        // print_info_client(*(it->second));
    }
}

#include <stdlib.h>  // <cstdlib> en C++
/**
 * @brief function to extract a valid command from the buffer.
 * @param buffer, string to extract the command.
 * @return a valid command.
 */
static std::string    extractCommand(std::string& buffer)
{
    std::string     res;
    size_t          pos = buffer.find("\n");

    // PRINT("STR BUFFER: ", YELLOW, "");
    // PRINT(buffer, WHITE, "\n");
    // PRINT("Pos: ", BLUE, "\n");
    // PRINT(pos, RED, "\n");
    if (pos != 0)
    {
        // PRINT("inside if pos", RED, "\n");
        res = buffer.substr(0, pos + 1);
        buffer.erase(0, pos + 1);
    }
    int size = res.size();
    // std::cout << "res: "  << res << "size: " << size << std::endl;
    // if (res[size - 1] == '\n')
    // {
    //     std::cout << "new_line present" << std::endl;
    // }
    // if (res[size - 2] == '\r')
    // {
    //     std::cout << "carriage present" << std::endl;
    // }
    if (size <= 2 && res[size - 1] != '\n' && res[size - 2] != '\r')
        throw std::runtime_error("Not carriage or newline at the end of the command");
    res = res.substr(0, size - 2);
    // PRINT("STR COMMAND: ", YELLOW, "");
    // PRINT(res, RED, "\n");
    return (res);
}
