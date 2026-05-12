#ifndef PART_COMMAND_HPP
# define PART_COMMAND_HPP

# include "Command.hpp"
# include "Server.hpp"

class PartCommand : public Command
{
public:
	PartCommand(const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > params);
	~PartCommand();

	void	execute(Server &server) const;

private:
	static const unsigned int min_params = 1;
	static const unsigned int max_params = 0; //NOTE: no max params for PART command, but we need to set it to something for the constructor to work. Setting it to 0 means "no max params"
};

#endif
