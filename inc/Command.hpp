#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <ostream>
#include <string>
#include <vector>

class Command
{
	public:
	enum commandType
	{
		EMPTY,
		JOIN,
		PRIVMSG,
		KICK,
		INVITE,
		TOPIC,
		MODE,
		WHO,
		PASS,
		NICK,
		USER,
		PART
	};

	private:
	int							_clientFd;
	commandType					_commandType;
	std::vector<std::string>	_params;

	public:
	// CONSTRUCTOR
	Command();
	Command(int clientFd, commandType type, std::vector<std::string> const &params);
	~Command();
	Command(Command const &original);

	// OPERATOR
	Command &operator=(Command const &other);

	// GETTERS
	int getClientFd() const;
	commandType getCommandType() const;
	std::vector<std::string> const &getParams() const;

	// SETTERS
	void setClientFd(int clientFd);
	void setCommandType(commandType type);
	void setParams(std::vector<std::string> const &params);

	// HELPER
	std::string commandTypeToString() const;
};
std::ostream &operator<<(std::ostream &o, const Command &obj);

#endif // !COMMAND_HPP
