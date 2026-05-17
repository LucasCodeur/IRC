#include "Server.hpp"
#include "Command.hpp"
#include "debug.hpp"
#include "CommandFactory.hpp"

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
    // Client temp;
    //
    // temp.setFd(clientFd);
    // this->_clients.insert(std::pair<int, Client>(clientFd, temp));
    int bytes_read;
    char buffer[BUFFER_SIZE] = {"0"};
    std::string stringBuf;
    std::string strCommand;

    while (1)
    {
        Command* command;
        bytes_read = recv(clientFd, buffer, sizeof(buffer), 0);
        buffer[bytes_read] = '\0';
        PRINT("received: ", GREEN, "");
        PRINT(clientFd, GREEN, "\n");
        PRINT(buffer, GREEN, "\n");
        stringBuf += buffer;
        strCommand = extractCommand(stringBuf);
        memset(buffer, 0, BUFFER_SIZE);
        try
        {
            command = CommandFactory::createCommand(clientFd, strCommand);
            command->execute(*this);
        }
        catch(Command::UnknownCommandException& e)
        {
            std::cout << "Caught: " << e.what() << std::endl;
            continue ;
        }
        catch(Command::EmptyCommandException& e)
        {
            std::cout << "Caught: " << e.what() << std::endl;
            return ;
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
    }
}

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
        res = buffer.substr(0, pos - 1);
        buffer.erase(0, pos + 1);
    }
    // PRINT("STR COMMAND: ", YELLOW, "");
    // PRINT(res, RED, "\n");
    return (res);
}
