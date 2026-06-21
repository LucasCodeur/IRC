#ifndef INVITE_COMMAND_HPP
# define INVITE_COMMAND_HPP

# include "Command.hpp"
# include "Server.hpp"

class InviteCommand : public Command
{
public:
	InviteCommand(Server *server, const int clientFd, t_msgSpecs specs, const std::vector<std::vector<std::string> > params);
	~InviteCommand();

	void	execute() const;

private:
	static const unsigned int min_params = 2;
	static const unsigned int max_params = 2;
	std::string	_invitedNick;
	std::string	_targetChannel;

	//METHODS
	void	confirmInvite(Client const &client, Channel const &channel)const;

};

#endif
