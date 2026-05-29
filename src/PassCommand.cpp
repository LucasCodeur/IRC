#include <sys/socket.h>

#include "PassCommand.hpp"
#include "Exceptions.hpp"

PassCommand::PassCommand(Server *server, const int clientFd, Command::t_msgSpecs specs, const std::vector<std::vector<std::string> > params) : Command(server, clientFd, specs, params)
{
	if (params.size() < PassCommand::min_params)
		throw Command::IncorrectParametersException("Not enough parameters");
	else if (params.size() > PassCommand::max_params)
		throw Command::IncorrectParametersException("Too much parameters");
}

PassCommand::~PassCommand() {};

void	PassCommand::execute() const
{
	// std::cout << "Inside execute pass command\n" << std::endl;
	std::string password = this->_params[0][0];
	// std::cout << "Password: " << password << std::endl;
	// std::cout << "size: " << password.size() << std::endl; 
	// std::cout << "server pass_word: " << server.getPassword() << std::endl;
	// std::cout << "size: " << server.getPassword().size() << std::endl;

	if (password != _server->getPassword())
	{
		// std::cout << "problem password not correct" << std::endl;
		throw passwordNotCorrect();
	}

	std::string message = "Password is correct\n";
	// std::cout << "before to send" << std::endl;
	if (send(this->getClientFd(), message.c_str(), message.size(), 0) < 0)
		throw sendFailed();
}
