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

	it->second->authState.setFullyRegistered(true);
	it->second->setUsername(this->_params[0][0]);
	it->second->setRealname(this->_trailer);

	std::string client = it->second->getNickname();
	try 
	{
		std::string reply;

		reply = this->_director.rplWelcome(*(it->second));
		if (send(this->getClientFd(), reply.c_str(), reply.size(), 0) < 0)
			throw sendFailed();

		reply = this->_director.rplYourhost(*(it->second));
		if (send(this->getClientFd(), reply.c_str(), reply.size(), 0) < 0)
			throw sendFailed();

		reply = this->_director.rplCreated(*(it->second));
		if (send(this->getClientFd(), reply.c_str(), reply.size(), 0) < 0)
			throw sendFailed();

		reply = this->_director.rplMyInfo(*(it->second));
		if (send(this->getClientFd(), reply.c_str(), reply.size(), 0) < 0)
			throw sendFailed();
	}
	catch (std::exception& e)
	{
		std::cout << "Caught: " << e.what() << std::endl;
			return ;
	}
}
