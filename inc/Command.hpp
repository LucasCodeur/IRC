#ifndef COMMAND_HPP
#define COMMAND_HPP

#define COMMAND_TYPES_AMOUNT 11 // TODO: is it the right way to do this?

#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

class Command
{

	class UnknownCommandException : public std::runtime_error
	{
		public:
			UnknownCommandException();
			UnknownCommandException(std::string msg);
			~UnknownCommandException() throw();
	};

	class EmptyCommandException : public std::runtime_error
	{
		public:
			EmptyCommandException();
			EmptyCommandException(std::string msg);
			~EmptyCommandException() throw();
	};

	class IncorrectParametersException : public std::runtime_error
	{
		public:
			IncorrectParametersException();
			IncorrectParametersException(std::string msg);
			~IncorrectParametersException() throw();
	};

	public:
	enum commandType
	{
		EMPTY = 0,
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
	Command(int clientFd, std::string str);
	Command(int clientFd, commandType type, std::vector<std::string> const &params);
	~Command();
	Command(Command const &original);

	// OPERATOR
	Command &operator=(Command const &other);

	// GETTERS
	int getClientFd() const;
	commandType getCommandType() const;
	std::vector<std::string> const &getParams() const;
};
std::ostream &operator<<(std::ostream &o, const Command &obj);

#endif // !COMMAND_HPP
