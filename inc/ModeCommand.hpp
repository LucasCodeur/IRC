#ifndef MODE_COMMAND_HPP
# define MODE_COMMAND_HPP

#include "Server.hpp"
#include "Command.hpp"

class ModeCommand : public Command
{
public:
	ModeCommand(Server *server, const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > params);
	~ModeCommand();
	enum editionType { ADD = 0, REMOVE, UNKNOWNOPERATION};
	enum editedMode { INVITEONLY = 0, CHANOPRIVFORTOPIC, KEY, OP, USERLIMIT, UNKNOWNMODE};

	void execute() const;
	void replyChannelMode() const;

private:
	static const unsigned int	min_params = 1;
	static const unsigned int	max_params = 2;
	std::string					operationChar;
	Channel *					_targetChannel;
};

#endif
