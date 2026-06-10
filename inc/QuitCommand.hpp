#ifndef QUIT_COMMAND_HPP
# define QUIT_COMMAND_HPP

#include "Command.hpp"
#include "Server.hpp"

class QuitCommand : public Command
{

public:
	QuitCommand(Server *server, const int clientFd, t_msgSpecs specs, const std::vector <std::vector<std::string> > params);
	~QuitCommand();

	void	execute() const;

private:
	static const unsigned int min_params = 0;
	static const unsigned int max_params = 0;
};

#endif
