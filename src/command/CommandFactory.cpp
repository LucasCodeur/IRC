#include "CommandFactory.hpp"
#include "JoinCommand.hpp"
#include "ModeCommand.hpp"
#include "TopicCommand.hpp"
#include "PassCommand.hpp"
#include "NickCommand.hpp"
#include "KickCommand.hpp"
#include "UserCommand.hpp"
#include "PartCommand.hpp"
#include "InviteCommand.hpp"
#include "ModeCommand.hpp"
#include "QuitCommand.hpp"
#include "PrivmsgCommand.hpp"
#include "WhoCommand.hpp"
#include "Command.hpp"
#include "debug.hpp"
#include "string.h"

#include <sstream>
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
	if (DEBUG)
		std::cout << "	creating command from : '" << str << "'" << std::endl;

	const char		*types[13] = {"", "JOIN", "PRIVMSG", "KICK", "INVITE", "TOPIC", "MODE", "WHO", "PASS", "NICK", "USER", "PART", "QUIT"};
	const size_t	array_size = sizeof(types) / sizeof(char *);

	Command *(*creators[13])(Server *server, const int clientFd, Command::t_msgSpecs specs, const std::vector<std::vector<std::string> > arguments);

	for (unsigned int i = 0; i < array_size; i++)
		creators[i] = NULL;
	creators[1] = &CommandFactory::createJoinCommand;
	creators[2] = &CommandFactory::createPrivmsgCommand;
	creators[3] = &CommandFactory::createKickCommand;
	creators[4] = &CommandFactory::createInviteCommand;
	creators[5] = &CommandFactory::createTopicCommand;
	creators[6] = &CommandFactory::createModeCommand;
	creators[7] = &CommandFactory::createWhoCommand;
	creators[8] = &CommandFactory::createPassCommand;
	creators[9] = &CommandFactory::createNickCommand;
	creators[10] = &CommandFactory::createUserCommand;
	creators[11] = &CommandFactory::createPartCommand;
	creators[12] = &CommandFactory::createQuitCommand;

	std::vector<std::string>	commandTypes(types, types + array_size);
	std::vector<std::string>	formattedMessage;
	std::string					stringSlice = "";
	std::string					prefix = "";
	std::string					command = "";
	std::vector<std::vector<std::string> > arguments;
	std::string					trailer = "";
	size_t						type = 0;
	size_t						prefixEndIndex = 0;

	if (str.length() == 0)
		throw Command::EmptyCommandException();
	if (str[0] == ':')
	{
		prefixEndIndex = str.find(' ');
		prefix = str.substr(0, prefixEndIndex);
	}

	size_t	colon_pos = str.find(':', prefixEndIndex);
	if (colon_pos != str.npos)
		trailer = str.substr(colon_pos + 1);
	
	std::vector<std::string> trailerContainer;
	trailerContainer.push_back(trailer);


	std::stringstream ss;
	ss << (str.substr(prefixEndIndex, colon_pos));

	while(getline(ss, stringSlice, ' '))
	{
		size_t pos = stringSlice.find(" ");
		if (pos != std::string::npos)
			stringSlice.erase(pos, 1);
		formattedMessage.push_back(stringSlice);
	}

	if (formattedMessage.size() == 0)
		throw Command::EmptyCommandException();
	command = formattedMessage[0];

	if (DEBUG)
	{
		std::cerr << "		whole command : " << str << std::endl;
		std::cerr << "		prefix : " << prefix << std::endl;
		std::cerr << "		Command : " << command << std::endl;
		std::cerr << "		trailer : " << trailer << std::endl;
	}
	
	// Authstate as = server->getClient(clientFd)->getAuthstate();
	// if (command != "PASS" && command != "USER" && command != "NICK" && as.getFullyRegistered())
	// 	return NULL;

	Command::t_msgSpecs msgSpecs;

	msgSpecs.prefix = prefix;
	msgSpecs.command = command;
	msgSpecs.trailer =  trailer;

	for (std::vector<std::string>::iterator it = formattedMessage.begin() + 1; it != formattedMessage.end(); ++it)
	{
		std::vector<std::string> group = split(*it, ',');
		if (!group.empty())
		{
			arguments.push_back(group);
		}
	}

	for (type = 0; type < commandTypes.size(); ++type)
	{
		if (formattedMessage[0] == commandTypes[type])
		{
			if (creators[type] == NULL)
				throw Command::UnknownCommandException(std::string(formattedMessage[0]));
			return (*creators[type])(server, clientFd, msgSpecs, arguments);
		}
	}
	throw Command::UnknownCommandException(std::string(formattedMessage[0]));
}


Command *CommandFactory::createJoinCommand(Server *server, const int clientFd, Command::t_msgSpecs specs, const std::vector<std::vector<std::string> > params)
{
	return (new JoinCommand(server, clientFd, specs, params));
}

Command *CommandFactory::createPrivmsgCommand(Server *server, const int clientFd, Command::t_msgSpecs specs, const std::vector<std::vector<std::string> > params)
{
	return (new PrivmsgCommand(server, clientFd, specs, params));
}

Command *CommandFactory::createInviteCommand(Server *server, const int clientFd, Command::t_msgSpecs specs, const std::vector<std::vector<std::string> > params)
{
	return (new InviteCommand(server, clientFd, specs, params));
}

Command *CommandFactory::createKickCommand(Server *server, const int clientFd, Command::t_msgSpecs specs, const std::vector<std::vector<std::string> >params)
{
	return (new KickCommand(server, clientFd, specs, params));
}

Command *CommandFactory::createTopicCommand(Server *server, const int clientFd, Command::t_msgSpecs specs, const std::vector<std::vector<std::string> >params)
{
	return (new TopicCommand(server, clientFd, specs, params));
}

Command *CommandFactory::createModeCommand(Server *server, const int clientFd, Command::t_msgSpecs specs, const std::vector<std::vector<std::string> > params)
{
	return (new ModeCommand(server, clientFd, specs, params));
}

Command *CommandFactory::createWhoCommand(Server *server, const int clientFd, Command::t_msgSpecs specs, const std::vector<std::vector<std::string> >params)
{
	return (new WhoCommand(server, clientFd, specs, params));
}

Command *CommandFactory::createPassCommand(Server *server, const int clientFd, Command::t_msgSpecs specs, const std::vector<std::vector<std::string> >params)
{
	return (new PassCommand(server, clientFd, specs, params));
}

Command *CommandFactory::createNickCommand(Server *server, const int clientFd, Command::t_msgSpecs specs, const std::vector<std::vector<std::string> >params)
{
	return (new NickCommand(server, clientFd, specs, params));
}

Command *CommandFactory::createUserCommand(Server *server, const int clientFd, Command::t_msgSpecs specs, const std::vector<std::vector<std::string> > params)
{
	return (new UserCommand(server, clientFd, specs, params));
}

Command *CommandFactory::createPartCommand(Server *server, const int clientFd, Command::t_msgSpecs specs, const std::vector<std::vector<std::string> > params)
{
	return (new PartCommand(server, clientFd, specs, params));
}

Command *CommandFactory::createQuitCommand(Server *server, const int clientFd, Command::t_msgSpecs specs, const std::vector<std::vector<std::string> > params)
{
	return (new QuitCommand(server, clientFd, specs, params));
}
