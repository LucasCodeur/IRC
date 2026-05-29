#include <iostream>
#include <vector>
#include "Command.hpp"
#include "PrivmsgCommand.hpp"

PrivmsgCommand::PrivmsgCommand(Server *server, const int clientFd, Command::t_msgSpecs specs, const std::vector<std::vector<std::string> > params) : Command(server, clientFd, specs, params)
{
	if (params.size() < PrivmsgCommand::min_params || this->_trailer.empty())
		throw Command::IncorrectParametersException("Not enough parameters");
}

PrivmsgCommand::~PrivmsgCommand() {}

void PrivmsgCommand::execute() const
{
	std::cout << "Executing PRIVMSG command from client " << std::endl;
}
