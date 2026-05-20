#ifndef NICK_COMMAND_HPP
# define NICK_COMMAND_HPP

#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"

class NickCommand : public Command
{
	public:
		NickCommand(Server *server, const int ClientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > params);
		~NickCommand();

		void	execute() const;
	private:
		static const unsigned int min_params = 1;
		static const unsigned int max_params = 1;
};

#endif
