#include <iostream>
#include <vector>
#include "Command.hpp"
#include "PrivmsgCommand.hpp"

PrivmsgCommand::PrivmsgCommand(Server *server, const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > params) : Command(server, clientFd, type, params)
{
	if (params.size() < PrivmsgCommand::min_params)
		throw Command::IncorrectParametersException("Not enough parameters");
	if (type != PRIVMSG)
		throw UnknownCommandException();
}

PrivmsgCommand::~PrivmsgCommand() {}

void PrivmsgCommand::execute() const
{
	std::cout << "Executing PRIVMSG command from client " << std::endl;
}
