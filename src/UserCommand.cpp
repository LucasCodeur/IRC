#include "Command.hpp"
#include "UserCommand.hpp"
#include "ReplyBuilder.hpp"
#include "debug.hpp"
#include "Exceptions.hpp"


UserCommand::UserCommand(Server *server, const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > params) : Command(server, clientFd, type, params)
{
	if (params.size() < UserCommand::min_params)
		throw Command::IncorrectParametersException("Not enough parameters");
	else if (params.size() > UserCommand::max_params)
		throw Command::IncorrectParametersException("Too much parameters");
	if (type != USER)
		throw UnknownCommandException(); //FIXME: use appropriate exception for this
}

UserCommand::~UserCommand() {};

void	UserCommand::execute() const
{
	std::map<int, Client*>::const_iterator it = _server->getClientmap().find(this->getClientFd());
	
	it->second->setUsername(this->_params[0][0]);
	it->second->setRealname(this->_params[3][0]);
	// PRINT("this->params[1][0]: ", GREEN, "");
	// PRINT(this->_params[1][0], GREEN, "\n");
	// PRINT("this->params[2][0]: ", GREEN, "");
	// PRINT(this->_params[2][0], GREEN, "\n");
	PRINT("this->params[3][0]: ", RED, "");
	PRINT(this->_params[3][0], WHITE, "\n");
	// PRINT("this->params[4][0]: ", GREEN, "");
	// PRINT(this->_params[4][0], GREEN, "\n");
	PRINT("RealName: ", RED, "");
	PRINT(it->second->getRealname(), WHITE, "\n");

	Director director;
	std::string message = "User information complete successfully\n";
	std::string client = it->second->getNickname();

	try 
	{
		std::string reply = director.rplWelcome(client);
		if (send(this->getClientFd(), reply.c_str(), reply.size(), 0) < 0)
			throw sendFailed();
		reply = director.rplYourhost(reply);
		if (send(this->getClientFd(), reply.c_str(), reply.size(), 0) < 0)
			throw sendFailed();
	}
	catch (std::exception& e)
	{
            std::cout << "Caught: " << e.what() << std::endl;
			return ;
	}
}
