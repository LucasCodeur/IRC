#ifndef WHO_COMMAND_HPP
# define WHO_COMMAND_HPP

# include "Command.hpp"
# include "Server.hpp"

class WhoCommand : public Command
{
	public:
		WhoCommand(Server *server, const int clientFd, t_msgSpecs specs, const std::vector<std::vector<std::string> > params);
		~WhoCommand();

		void	execute() const;
	private:
		static const unsigned int min_params = 1;
		static const unsigned int max_params = 1;
};

#endif
