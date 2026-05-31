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
	
	it->second->setUsername(this->_trailer);
	it->second->setRealname(this->_trailer);
	// PRINT("this->params[1][0]: ", GREEN, "");
	// PRINT(this->_params[1][0], GREEN, "\n");
	// PRINT("this->params[2][0]: ", GREEN, "");
	// PRINT(this->_params[2][0], GREEN, "\n");
	// PRINT("this->params[4][0]: ", GREEN, "");
	// PRINT(this->_params[4][0], GREEN, "\n");
	// PRINT("RealName: ", RED, "");
	// PRINT(it->second->getRealname(), WHITE, "\n");

	Director director;
	std::string message = "User information complete successfully\n";
	std::string client = it->second->getNickname();

	try 
	{
		// PRINT("Inside try to reply in USER", BLUE, "\n");
		std::string reply = director.rplWelcome(*(it->second));
		if (send(this->getClientFd(), reply.c_str(), reply.size(), 0) < 0)
			throw sendFailed();

		reply = director.rplYourhost(*(it->second));
		if (send(this->getClientFd(), reply.c_str(), reply.size(), 0) < 0)
			throw sendFailed();
	}
	catch (std::exception& e)
	{
        std::cout << "Caught: " << e.what() << std::endl;
		return ;
	}
}
