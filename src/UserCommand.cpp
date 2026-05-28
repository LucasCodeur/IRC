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
	
	if (it->second->authState.getNickReceived() == true)
	{
		it->second->authState.setFullyRegistered(true);
		it->second->setUsername(this->_params[0][0]);
		it->second->setRealname(this->_params[3][0]);
		Director director;
		std::string client = it->second->getNickname();

		try 
		{
			std::string reply = director.rplWelcome(*(it->second));
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
