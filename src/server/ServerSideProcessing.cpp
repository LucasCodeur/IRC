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
        if (this->getPassword().empty() == true)
            temp->getAuthstate().setPasswordReceived(true);
        temp->setFd(clientFd);
        this->_clients.insert(std::pair<int, Client*>(clientFd, temp));
    }

    int         bytes_read;
    char        buffer[BUFFER_SIZE] = {"0"};

    Client *client = this->getClient(clientFd);

    memset(buffer, 0, BUFFER_SIZE);
    bytes_read = recv(clientFd, buffer, sizeof(buffer), 0);
    if (bytes_read <= 0)
    {
        if (bytes_read == 0 || ((bytes_read == -1) && (errno != EAGAIN && errno != EWOULDBLOCK)))
        {
            PRINT("client disconnected: ", RED, "");
            PRINT(clientFd, RED, "\n");

            std::map<int, Client*>::iterator it = this->_clients.find(clientFd);
            print_info_client(*(it->second));

            delete it->second;
            this->_clients.erase(it);

            this->controlEpoll(EPOLL_CTL_DEL, clientFd, NULL);
            close(clientFd);

            // print_info_client(*(it->second));
            return ;
        }
    }
    buffer[bytes_read] = '\0';
    if (handleRequest(buffer, *client, clientFd) == true)
        return ;
}

/**
 * @brief allows handling of the client request.
 * @param buffer that contains the information sent by the client. 
 * @param client, object that contains all information about the client.
 * @return false if we have to continue to get information about the client or true if we have to stop.
 */
bool    Server::handleRequest(char* buffer, const Client& client, const int clientFd)
{
    bool        stop = false;
    Command*    command = NULL;
    std::string strCommand;

    try
    {
        std::map<int, Client*>::const_iterator  it = this->_clients.find(clientFd);
        std::string&                            clientBuffer = (*it).second->getBuf();

        clientBuffer += buffer;
        if (clientBuffer.size() == 0)
            return (true);

        strCommand = extractCommand(clientBuffer);
        command = CommandFactory::createCommand(this, clientFd, strCommand);
        command->execute();
    }
    catch(Command::UnknownCommandException& e)
    {
        std::cout << "Caught: " << e.what() << std::endl;
        std::string cmdKeyword = e.what();
        std::string reply = command->getDirector()->errUnknownCommand(client.getNickname(), cmdKeyword);
        this->sendData(clientFd, reply);
    }
    catch (Command::NotEnoughParametersException& e)
    {
        std::cout << "Caught: " << e.what() << std::endl;
        std::string reply = command->getDirector()->errNeedMoreParams(client.getNickname(), e.what());
        this->sendData(clientFd, reply);
    }
    catch(std::exception& e)
    {
        std::cout << "Caught: " << e.what() << std::endl;
        stop = true;
    }
    delete command;

    return (stop);
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

    if (pos != 0)
    {
        res = buffer.substr(0, pos);
        buffer.erase(0, pos + 1);
    }
    int size = res.size();
    if (res[size] != '\n' && res[size - 1] != '\r')
        throw std::runtime_error("Not carriage or newline at the end of the command");
    res = res.substr(0, size - 1);
    return (res);
}
