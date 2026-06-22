#include "Server.hpp"
#include "Command.hpp"
#include "CommandFactory.hpp"
#include "utils.hpp"
#include <exception>
#include <stdio.h>
#include <bits/stdc++.h>

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
				return (false);
			strCommand = utils_server::extractCommand(clientOutBuffer, true);
			if (strCommand.empty())
				return (false);
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
