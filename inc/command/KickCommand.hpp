#ifndef KICK_COMMAND_HPP
# define KICK_COMMAND_HPP

# include "Command.hpp"
# include "Server.hpp"

class KickCommand : public Command
{
public:
	KickCommand(Server *server, const int clientFd, t_msgSpecs specs, const std::vector<std::vector<std::string> > params);
	~KickCommand();

	void	execute() const;

private:
	static const unsigned int min_params = 2;
	static const unsigned int max_params = 2;
};

#endif
