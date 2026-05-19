#ifndef MODE_COMMAND_HPP
# define MODE_COMMAND_HPP

#include "Server.hpp"
#include "Command.hpp"

class ModeCommand : public Command
{
public:
	ModeCommand(const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > params);
	~ModeCommand();

	void execute(Server &server) const;

private:
	static const unsigned int min_params = 1;
	static const unsigned int max_params = 2;
};

#endif
