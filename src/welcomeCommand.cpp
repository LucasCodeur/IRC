#include <sstream>
#include <sys/socket.h>

#include "WelcomeCommand.hpp"
#include "Client.hpp"
#include "Exceptions.hpp"

WelcomeCommand::WelcomeCommand(Server *server, const int clientFd, Command::t_msgSpecs specs, const std::vector<std::vector<std::string> > params) : Command(server, clientFd, specs, params)
{
	if (params.size() < WelcomeCommand::min_params)
		throw NotEnoughParametersException("WELCOME");
	else if (params.size() > WelcomeCommand::max_params)
		throw Command::IncorrectParametersException("Too much parameters");
}

WelcomeCommand::~WelcomeCommand() {};

void	WelcomeCommand::execute() const
{
	std::stringstream ss(this->_params[0][0]);
	int clientFd;
	ss >> clientFd;
	std::map<int, Client*>::const_iterator	it = _server->getClientmap().find(clientFd);
	Client*									client = it->second;

	std::string		message = "BOT: Welcome to the best irc server of the world by enchevri, kbarru and lud-adam\r\n";

	client->addToBuffer(message);
}
