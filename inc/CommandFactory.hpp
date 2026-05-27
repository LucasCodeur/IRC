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
	static Command	*createTopicCommand(Server *server, const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > params);
	static Command	*createJoinCommand(Server *server, const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > params);
	// Command	*createKickCommand(const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > params) const;
	// Command	*createModeCommand(const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > params) const;
	// Command	*createWhoCommand(const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > params) const;
	static Command	*createPassCommand(Server *server, const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > params);
	static Command	*createNickCommand(Server *server, const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > params);
	static Command	*createUserCommand(Server *server, const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > params);
	static Command	*createPartCommand(Server *server,const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > params);
	static Command	*createPrivmsgCommand(Server *server, const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > params);
	static Command	*createModeCommand(Server *server, const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > params);

};

#endif
