#include "Server.hpp"
#include "Command.hpp"
#include "debug.hpp"
#include "CommandFactory.hpp"

#include <exception>
#include <stdexcept>
#include <stdio.h>
#include <errno.h>
#include <bits/stdc++.h>

static std::string	extractCommand(std::string& buffer);

/**
 * @brief allows handling of the client request.
 * @param buffer that contains the information sent by the client.
 * @param client, object that contains all information about the client.
 * @return false if we have to continue to get information about the client or true if we have to stop.
 */
bool	Server::handleRequest(Client& client)
{
	bool		stop = false;
	Command*	command = NULL;
	int			clientFd = client.getFd();

	while (!stop)
	{
		std::map<int, Client*>::const_iterator it = this->_clients.find(clientFd);
		if (it == this->_clients.end())
		{
			stop = true;
			continue ;
		}
		try
		{
			std::string&	clientOutBuffer = client.getBuf();
			std::string		strCommand;

			if (clientOutBuffer.size() == 0)
				return (true);
			strCommand = extractCommand(clientOutBuffer);
			if (strCommand.empty())
			{
				stop = true;
				continue ;
			}
			command = CommandFactory::createCommand(this, clientFd, strCommand);
			command->execute();

			delete command;
		}
		catch(Command::UnknownCommandException& e)
		{
			std::cout << "Caught: " << e.what() << std::endl;
			std::string cmdKeyword = e.what();
			std::string reply = command->getDirector()->errUnknownCommand(client.getNickname(), cmdKeyword);
			this->writeInBuffer(&client, reply);
			delete command;
		}
		catch (Command::NotEnoughParametersException& e)
		{
			std::cout << "Caught: " << e.what() << std::endl;
			std::string reply = command->getDirector()->errNeedMoreParams(client.getNickname(), e.what());
			this->writeInBuffer(&client, reply);
			delete command;
		}
		catch(std::exception& e)
		{
			std::cout << "Caught: " << e.what() << std::endl;
			stop = true;
		}
	}
	return (stop);
}

/**
 * @brief function to extract a valid command from the buffer.
 * @param buffer, string to extract the command.
 * @return a valid command.
 */
static std::string	extractCommand(std::string& buffer)
{
	std::string		res;
	size_t			pos = buffer.find("\n");

	if (pos != std::string::npos)
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
