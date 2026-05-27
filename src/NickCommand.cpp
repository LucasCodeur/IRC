#include "NickCommand.hpp"
#include "Client.hpp"
#include "Exceptions.hpp"
#include "debug.hpp"

NickCommand::NickCommand(Server *server, const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > params) : Command(server, clientFd, type, params)
{
	size_t sizeParams = params.size();
	if (sizeParams < NickCommand::min_params)
		throw Command::IncorrectParametersException("Not enough parameters");
	else if (sizeParams > NickCommand::max_params)
		throw Command::IncorrectParametersException("Too much parameters");
	if (type != NICK)
		throw UnknownCommandException(); //FIXME: use appropriate exception for this
}

NickCommand::~NickCommand() {};

void	NickCommand::execute() const
{
	std::string nickname = this->_params[0][0];
	std::map<int, Client*>::const_iterator it = _server->getClientmap().find(this->getClientFd());
	it->second->setNickname(nickname);
	// std::string message = "Nick information complete successfully\n";
	// if (send(this->getClientFd(), message.c_str(), message.size(), 0) < 0)
	// 	throw sendFailed();
}
