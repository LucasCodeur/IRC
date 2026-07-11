#include <sys/socket.h>

#include "WelcomeCommand.hpp"

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
	std::string		reply = "BOT: Welcome to the best irc server of the world by enchevri, kbarru and lud-adam\r\n";
	this->_server->writeInBuffer(this->_client, reply);
}
