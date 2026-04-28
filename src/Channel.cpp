#include "Channel.hpp"
#include <algorithm>
#include "debug.hpp"
#include <iostream>

Channel::Channel()
	: _name(""),
	  _topic(""),
	  _password(""),
	  _mode(0)
{
	std::cout << DEBUG GREEN "Channel created: " RESET << *this <<std::endl;
}

Channel::Channel(std::string const &name)
	: _name(name),
	  _topic(""),
	  _password(""),
	  _mode(0)
{
	std::cout << DEBUG GREEN "Channel created: " RESET << *this <<std::endl;
}

Channel::~Channel()
{
	for (size_t i = 0; i < _users.size(); ++i)
	{
		std::cout << DEBUG YELLOW "Removing user " RESET << _users[i] << " from channel " << _name << std::endl;
	}
	std::cout << DEBUG RED "Channel destroyed: " RESET << *this <<std::endl;
}

Channel::Channel(Channel const &original)
	: _name(original._name),
	  _topic(original._topic),
	  _password(original._password),
	  _users(original._users),
	  _operators(original._operators),
	  _invited(original._invited),
	  _mode(original._mode)
{
	std::cout << DEBUG BLUE "Channel copied: " RESET << *this <<std::endl;
}

Channel &Channel::operator=(Channel const &other)
{
	if (this != &other)
	{
		this->_name = other._name;
		this->_topic = other._topic;
		this->_password = other._password;
		this->_users = other._users;
		this->_operators = other._operators;
		this->_invited = other._invited;
		this->_mode = other._mode;
		// std::cout << DEBUG BLUE "Channel assigned: " RESET << *this << std::endl;
	}
	return (*this);
}

std::ostream &operator<<(std::ostream &o, const Channel &obj)
{
	return (o << "Channel: " << obj.getName() 
			  << " (Topic: " << obj.getTopic()
			  << ", Users: " << obj.getUsers().size()
			  << ", Operators: " << obj.getOperators().size()
			  << ", Invited: " << obj.getInvited().size() 
			  << ", Mode: " << obj.getMode() << ")");
}

std::string const &Channel::getName() const
{
	return (_name);
}

std::string const &Channel::getTopic() const
{
	return (_topic);
}

std::string const &Channel::getPassword() const
{
	return (_password);
}

std::vector<int> const &Channel::getUsers() const
{
	return (_users);
}

std::vector<int> const &Channel::getOperators() const
{
	return (_operators);
}

std::vector<int> const &Channel::getInvited() const
{
	return (_invited);
}

int Channel::getMode() const
{
	return (_mode);
}

void Channel::setTopic(std::string const &topic)
{
	this->_topic = topic;
}

void Channel::setPassword(std::string const &password)
{
	this->_password = password;
}

void Channel::setMode(int mode)
{
	this->_mode = mode;
}

bool Channel::addUser(int clientFd)
{
	if (std::find(this->_users.begin(), this->_users.end(), clientFd) == this->_users.end())
	{
		this->_users.push_back(clientFd);
		return (true);
	}
	return (false);
}

void Channel::removeUser(int clientFd)
{
	std::vector<int>::iterator it = std::find(this->_users.begin(), this->_users.end(), clientFd);
	if (it != this->_users.end())
	{
		this->_users.erase(it);
	}
}

bool Channel::addOperator(int clientFd)
{
	if (std::find(this->_operators.begin(), this->_operators.end(), clientFd) == this->_operators.end())
	{
		this->_operators.push_back(clientFd);
		return (true);
	}
	return (false);
}

bool Channel::removeOperator(int clientFd)
{
	if (std::find(this->_operators.begin(), this->_operators.end(), clientFd) != this->_operators.end())
	{
		this->_operators.erase(std::remove(this->_operators.begin(), this->_operators.end(), clientFd), this->_operators.end());
		return (true);
	}
	return (false);
}

