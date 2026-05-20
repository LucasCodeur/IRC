#include "CommandFactory.hpp"
#include "JoinCommand.hpp"
#include "TopicCommand.hpp"
#include "PassCommand.hpp"
#include "NickCommand.hpp"
#include "UserCommand.hpp"
#include "Command.hpp"

#include "string.h"

#include <iostream>
#include <sstream>

CommandFactory::CommandFactory() {}

std::vector<std::string> split(std::string str,   char delimiter)
{
	std::vector<std::string>	v;
	std::string::size_type		curPos;
	std::string::size_type		delimPos;

	curPos = 0;
	delimPos = 0;
	while(delimPos != std::string::npos && curPos != std::string::npos)
	{
		delimPos = str.find(delimiter, curPos);
		std::string curStr = str.substr(curPos, delimPos - curPos);
		if (curStr.length() > 0)
			v.push_back(curStr);
		curPos = delimPos + 1;
	}

	return v;
}

Command *CommandFactory::createCommand(const int clientFd, const std::string str)
{
	std::cout << "Command :" << str << std::endl;
	const char *types[12] = {"", "JOIN", "PRIVMSG", "KICK", "INVITE", "TOPIC", "MODE", "WHO", "PASS", "NICK", "USER", "PART"};

	Command *(*creators[12])(const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > parameters);

	creators[0] = NULL;
	creators[1] = &CommandFactory::createJoinCommand;
	// creators[2] = &CommandFactory::createPrivmsgCommand;
	// creators[3] = &CommandFactory::createKickCommand;
	// creators[4] = &CommandFactory::createInviteCommand;
	creators[5] = &CommandFactory::createTopicCommand;
	// creators[6] = &CommandFactory::createModeCommand;
	// creators[7] = &CommandFactory::createWhoCommand;
	creators[8] = &CommandFactory::createPassCommand;
	creators[9] = &CommandFactory::createNickCommand;
	creators[10] = &CommandFactory::createUserCommand;
	// creators[11] = &CommandFactory::createPartCommand;

	std::vector<std::string>	commandTypes(types, types + COMMAND_TYPES_AMOUNT);
	std::vector<std::string>	formattedCommand;
	std::string					mainContent = "";
	std::string					splitString;
	size_t						type;

	size_t	colon_pos = str.find(':');
	if (colon_pos != str.npos)
		mainContent = str.substr(colon_pos + 1);
	
	std::vector<std::string> mainContentContainer;
	mainContentContainer.push_back(mainContent);


	std::stringstream ss(str.substr(0, colon_pos));

	while(getline(ss, splitString, ' '))
	{
		size_t pos = splitString.find(" ");
		if (pos != std::string::npos)
			splitString.erase(pos, pos);
		formattedCommand.push_back(splitString);
	}
	if (formattedCommand.size() <= 0)
		throw Command::EmptyCommandException();
	std::vector<std::vector<std::string> > parameters;

	for (std::vector<std::string>::iterator it = formattedCommand.begin() + 1; it != formattedCommand.end(); ++it)
	{
		parameters.push_back(split(*it, ','));
	}

	if (!mainContent.empty())
		parameters.push_back(mainContentContainer);

	for (type = 0; type < commandTypes.size(); ++type)
	{
		if (formattedCommand[0] == commandTypes[type])
		{
			return (*creators[type])(clientFd, (enum Command::commandType)type, parameters);
			break ;
		}
		if (type == commandTypes.size() - 1)
			throw Command::UnknownCommandException();
	}
	throw Command::UnknownCommandException();
}

Command *CommandFactory::createJoinCommand(const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > params)
{
	return (new JoinCommand(clientFd, type, params));
}

// Command *createPrivmsgCommand(const int clientFd, const enum Command::commandType type, const std::vector<std::string> params)
// {
// 	return (new PrivmsgCommand(clientFd, type, params));
// }

// Command *createInviteCommand(const int clientFd, const enum Command::commandType type, const std::vector<std::string> params)
// {
// 	return (new InviteCommand(clientFd, type, params));
// }

Command *CommandFactory::createTopicCommand(const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> >params)
{
	return (new TopicCommand(clientFd, type, params));
}

// Command *createKickCommand(const int clientFd, const enum Command::commandType type, const std::vector<std::string> params)
// {
// 	return (new KickCommand(clientFd, type, params));
// }

// Command *createModeCommand(const int clientFd, const enum Command::commandType type, const std::vector<std::string> params)
// {
// 	return (new ModeCommand(clientFd, type, params));
// }

// Command *createWhoCommand(const int clientFd, const enum Command::commandType type, const std::vector<std::string> params)
// {
// 	return (new WhoCommand(clientFd, type, params));
// }

Command *CommandFactory::createPassCommand(const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> >params)
{
	return (new PassCommand(clientFd, type, params));
}

Command *CommandFactory::createNickCommand(const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> >params)
{
	return (new NickCommand(clientFd, type, params));
}

Command *CommandFactory::createUserCommand(const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > params)
{
	return (new UserCommand(clientFd, type, params));
}

// Command *createPartCommand(const int clientFd, const enum Command::commandType type, const std::vector<std::string> params)
// {
// 	return (new PartCommand(clientFd, type, params));
// }
