#include "CommandFactory.hpp"
#include "JoinCommand.hpp"
#include "TopicCommand.hpp"
#include "PassCommand.hpp"
#include "NickCommand.hpp"
#include "UserCommand.hpp"
#include "PartCommand.hpp"
#include "PrivmsgCommand.hpp"
#include "Command.hpp"
#include <sstream>
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

Command *CommandFactory::createCommand(Server *server, const int clientFd, const std::string str)
{
	std::cout << "Command :" << str << std::endl;
	const char *types[12] = {"", "JOIN", "PRIVMSG", "KICK", "INVITE", "TOPIC", "MODE", "WHO", "PASS", "NICK", "USER", "PART"};

	Command *(*creators[12])(Server *server, const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > parameters);

	for (int i = 0; i < 12; i++)
		creators[i] = NULL;
	creators[1] = &CommandFactory::createJoinCommand;
	creators[2] = &CommandFactory::createPrivmsgCommand;
	creators[5] = &CommandFactory::createTopicCommand;
	creators[8] = &CommandFactory::createPassCommand;
	creators[9] = &CommandFactory::createNickCommand;
	creators[10] = &CommandFactory::createUserCommand;
	creators[11] = &CommandFactory::createPartCommand;

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


	std::stringstream ss;
	ss << (str.substr(0, colon_pos));

	while(getline(ss, splitString, ' '))
	{
		size_t pos = splitString.find(" ");
		if (pos != std::string::npos)
			splitString.erase(pos, 1);
		formattedCommand.push_back(splitString);
	}

	if (formattedCommand.size() <= 0)
	{
		std::cerr << "Formatted command size :" << formattedCommand.size() << std::endl << "first element : " << formattedCommand.front() << std::endl;
		throw Command::EmptyCommandException();
	}
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
			if (creators[type] == NULL)
				throw Command::UnknownCommandException();
			return (*creators[type])(server, clientFd, (enum Command::commandType)type, parameters);
		}
		if (type == commandTypes.size() - 1)
			throw Command::UnknownCommandException();
	}
	throw Command::UnknownCommandException();
}


Command *CommandFactory::createJoinCommand(Server *server, const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > params)
{
	return (new JoinCommand(server, clientFd, type, params));
}

Command *CommandFactory::createPrivmsgCommand(Server *server, const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > params)
{
	return (new PrivmsgCommand(server, clientFd, type, params));
}

// Command *createInviteCommand(const int clientFd, const enum Command::commandType type, const std::vector<std::string> params)
// {
// 	return (new InviteCommand(clientFd, type, params));
// }

Command *CommandFactory::createTopicCommand(Server *server, const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> >params)
{
	return (new TopicCommand(server, clientFd, type, params));
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

Command *CommandFactory::createPassCommand(Server *server, const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> >params)
{
	return (new PassCommand(server, clientFd, type, params));
}

Command *CommandFactory::createNickCommand(Server *server, const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> >params)
{
	return (new NickCommand(server, clientFd, type, params));
}

Command *CommandFactory::createUserCommand(Server *server, const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > params)
{
	return (new UserCommand(server, clientFd, type, params));
}

Command *CommandFactory::createPartCommand(Server *server, const int clientFd, const enum Command::commandType type, const std::vector<std::vector<std::string> > params)
{
	return (new PartCommand(server, clientFd, type, params));
}
