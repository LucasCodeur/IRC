/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbarru <kbarru@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 16:06:35 by kbarru            #+#    #+#             */
/*   Updated: 2026/05/07 16:10:13 by kbarru           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
#define COMMAND_HPP

#define COMMAND_TYPES_AMOUNT 11 // TODO: is it the right way to do this?

#include <ostream>
#include <string>
#include <vector>
#include "Server.hpp"

class Command
{


	public:
	enum commandType
	{
		EMPTY = 0,
		JOIN, // Enzo
		PRIVMSG,
		KICK,
		INVITE,
		TOPIC, // Killian
		MODE, // Killian
		WHO, // Enzo
		PASS, // Lucas
		NICK, // Lucas
		USER, // Lucas
		PART // Enzo
	};

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

	// CONSTRUCTOR
	Command();
	Command(int clientFd, commandType type, std::vector<std::vector<std::string> > const &params);
	virtual ~Command();
	Command(Command const &original);

	// OPERATOR
	Command &operator=(Command const &other);

	// GETTERS
	int getClientFd() const;
	commandType getCommandType() const;
	std::vector<std::vector<std::string> > const &getParams() const;

	// SETTERS
	void setClientFd(int clientFd);
	void setCommandType(commandType type);
	void setParams(std::vector<std::vector<std::string> > const &params);

	// HELPER
	std::string commandTypeToString() const;

	// METHODS
	virtual void	execute(Server &server) const = 0;
	

private:
	int							_clientFd;
	commandType					_commandType;

protected:
	std::vector<std::vector<std::string> >	_params;

};
std::ostream &operator<<(std::ostream &o, const Command &obj);

#endif // !COMMAND_HPP
