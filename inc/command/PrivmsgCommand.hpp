#ifndef PRIVMSG_COMMAND_HPP
# define PRIVMSG_COMMAND_HPP

# include "Command.hpp"
# include "Server.hpp"

class PrivmsgCommand : public Command
{
public:
	PrivmsgCommand(Server *server, const int clientFd, t_msgSpecs specs, const std::vector<std::vector<std::string> > params);
	~PrivmsgCommand();

	void	execute() const;

private:
	static const unsigned int min_params = 1;
};

#endif
