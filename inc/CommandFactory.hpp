#ifndef COMMAND_FACTORY_HPP
# define COMMAND_FACTORY_HPP

#include "Command.hpp"

class CommandFactory
{
public:
	CommandFactory();
	virtual ~CommandFactory() = 0;
	static Command *createCommand(Server *server, const int clientFd, const std::string str);

private:
	static Command	*createJoinCommand(Server *server, const int clientFd, Command::t_msgSpecs specs, const std::vector<std::vector<std::string> > params);
	static Command	*createPrivmsgCommand(Server *server, const int clientFd, Command::t_msgSpecs specs, const std::vector<std::vector<std::string> > params);
	// static Command	*createKickCommand(Server *server, const int clientFd, Command::t_msgSpecs specs, const std::vector<std::vector<std::string> > params) ;
	static Command	*createInviteCommand(Server *server, const int clientFd, Command::t_msgSpecs specs, const std::vector<std::vector<std::string> > params);
	static Command	*createTopicCommand(Server *server, const int clientFd, Command::t_msgSpecs specs, const std::vector<std::vector<std::string> > params);
	static Command	*createModeCommand(Server *server, const int clientFd, Command::t_msgSpecs specs, const std::vector<std::vector<std::string> > params);
	static Command	*createWhoCommand(Server *server, const int clientFd, Command::t_msgSpecs specs, const std::vector<std::vector<std::string> > params) ;
	static Command	*createPassCommand(Server *server, const int clientFd, Command::t_msgSpecs specs, const std::vector<std::vector<std::string> > params);
	static Command	*createNickCommand(Server *server, const int clientFd, Command::t_msgSpecs specs, const std::vector<std::vector<std::string> > params);
	static Command	*createUserCommand(Server *server, const int clientFd, Command::t_msgSpecs specs, const std::vector<std::vector<std::string> > params);
	static Command	*createPartCommand(Server *server,const int clientFd, Command::t_msgSpecs specs, const std::vector<std::vector<std::string> > params);
};

#endif
