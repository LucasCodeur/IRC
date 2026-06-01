#ifndef JOIN_COMMAND_HPP
# define JOIN_COMMAND_HPP

# include "Command.hpp"
# include "Server.hpp"

class JoinCommand : public Command
{
public:
	JoinCommand(Server *server, const int clientFd, t_msgSpecs specs, const std::vector<std::vector<std::string> > params);
	~JoinCommand();
	void sendNames(Channel *channel) const;

	void	execute() const;

private:
	static const unsigned int min_params = 1;
	static const unsigned int max_params = 2;
};

#endif
