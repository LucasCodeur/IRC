#include "Command.hpp"
#include "UserCommand.hpp"
#include "debug.hpp"
#include "Exceptions.hpp"


UserCommand::UserCommand(const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > params) : Command(clientFd, type, params)
{
	if (params.size() < UserCommand::min_params)
		throw Command::IncorrectParametersException("Not enough parameters");
	else if (params.size() > UserCommand::max_params)
		throw Command::IncorrectParametersException("Too much parameters");
	if (type != USER)
		throw UnknownCommandException(); //FIXME: use appropriate exception for this
}

UserCommand::~UserCommand() {};

void	UserCommand::execute(Server& server) const
{
	std::map<int, Client*>::const_iterator it = server.getClientmap().find(this->getClientFd());
	
	it->second->setUsername(this->_params[0][0]);
	it->second->setRealname(this->_params[0][3]);

	std::string message = "User information complete successfully\n";
	if (send(this->getClientFd(), message.c_str(), message.size(), 0) < 0)
		throw sendFailed();
}
