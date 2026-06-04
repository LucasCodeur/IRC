#include "Command.hpp"
#include "UserCommand.hpp"
#include "ReplyBuilder.hpp"
#include "Exceptions.hpp"
#include <iostream>


UserCommand::UserCommand(Server *server, const int clientFd, Command::t_msgSpecs specs, const std::vector<std::vector<std::string> > params) : Command(server, clientFd, specs, params)
{
	if (params.size() < UserCommand::min_params || this->_trailer.empty())
		throw Command::IncorrectParametersException("Not enough parameters");
	else if (params.size() > UserCommand::max_params)
		throw Command::IncorrectParametersException("Too much parameters");
}

UserCommand::~UserCommand() {};

void	UserCommand::execute() const
{
	std::map<int, Client*>::const_iterator it = _server->getClientmap().find(this->getClientFd());
	if (it->second->authState.getFullyRegistered() == true)
	{
		std::string reply;
		reply = this->_director.errAlreadyRegistred();
		if (send(this->getClientFd(), reply.c_str(), reply.size(), 0) < 0)
			throw sendFailed();
		return ;
	}
	if (it->second->authState.getNickReceived() == true)
	{
		// PRINT("MESSAGE SPECS", BLUE, "\n");
		// PRINT(this->_prefix, BLUE, "\n");
		// PRINT(this->_command, BLUE, "\n");
		// PRINT(this->_trailer, BLUE, "\n");

		// PRINT("this->params[1][0]: ", GREEN, "");
		//      PRINT(this->_params[1][0], GREEN, "\n");
		//      PRINT("this->params[2][0]: ", GREEN, "");
		//      PRINT(this->_params[2][0], GREEN, "\n");
		//     PRINT("this->params[3][0]: ", RED, "");
		//      PRINT(this->_params[3][0], WHITE, "\n");
		//      PRINT("this->params[4][0]: ", GREEN, "");
		//     PRINT(this->_params[4][0], GREEN, "\n");
		it->second->authState.setFullyRegistered(true);
		it->second->setUsername(this->_params[0][0]);
		// WARN: Maybe change I'm not sure if is it normal to put trailer in order to set realname
		it->second->setRealname(this->_trailer);
		Director director;
		std::string client = it->second->getNickname();

		try 
		{
			std::string reply;
			reply = director.rplWelcome(*(it->second));
			if (send(this->getClientFd(), reply.c_str(), reply.size(), 0) < 0)
				throw sendFailed();

			reply = director.rplYourhost(*(it->second));
			if (send(this->getClientFd(), reply.c_str(), reply.size(), 0) < 0)
				throw sendFailed();


			reply = director.rplCreated(*(it->second));
			if (send(this->getClientFd(), reply.c_str(), reply.size(), 0) < 0)
				throw sendFailed();

			reply = director.rplMyInfo(*(it->second));
			if (send(this->getClientFd(), reply.c_str(), reply.size(), 0) < 0)
				throw sendFailed();
		}
		catch (std::exception& e)
		{
		    std::cout << "Caught: " << e.what() << std::endl;
				return ;
		}
	}
	else
		throw std::runtime_error("nickname not received\n");
}
