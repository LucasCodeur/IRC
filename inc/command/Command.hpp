#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include <vector>
#include "ReplyBuilder.hpp"

class Server;

class Command
{


	public:

	typedef struct s_mgsSpecs
	{
		std::string prefix;
		std::string command;
		std::string trailer;
	}				t_msgSpecs;

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

	class NotEnoughParametersException : public std::runtime_error
	{
		public:
			NotEnoughParametersException();
			NotEnoughParametersException(std::string msg);
			~NotEnoughParametersException() throw();
	};

	// CONSTRUCTOR
	Command(Server *server, int clientFd, t_msgSpecs specs, std::vector<std::vector<std::string> > const &params);
	virtual ~Command();
	Command(Command const &original);

	// OPERATOR
	Command &operator=(Command const &other);

	// GETTERS
	int getClientFd() const;
	Server *getServer() const;
	Director	*getDirector() ;
	std::vector<std::vector<std::string> > const &getParams() const;
	Client *getClient() const;

	// SETTERS
	void setClientFd(int clientFd);
	void setParams(std::vector<std::vector<std::string> > const &params);

	// METHODS
	virtual void	execute() const = 0;

private:
	Command();
	int							_clientFd;

protected:
	Director								_director;
	std::vector<std::vector<std::string> >	_params;
	Server									*_server;
	std::string								_prefix;
	std::string								_command;
	std::string								_trailer;
	Client									*_client;
};

#endif // !COMMAND_HPP
