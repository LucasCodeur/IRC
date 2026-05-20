#ifndef PASS_COMMAND_HPP
# define PASS_COMMAND_HPP

# include "Command.hpp"
# include "Server.hpp"

class PassCommand : public Command
{
	public:
		PassCommand(Server *server, const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > params);
		~PassCommand();

		void	execute() const;
	private:
		static const unsigned int min_params = 1;
		static const unsigned int max_params = 1;
};

#endif
