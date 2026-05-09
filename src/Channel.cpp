#include "Channel.hpp"
#include "debug.hpp"
#include <iostream>

Channel::Channel()
	: _name(""),
	  _topic(""),
	  _password(""),
	  _mode(0)
{
	// std::cout << GREEN "Channel created: " RESET << *this <<std::endl;
}

Channel::~Channel()
{
	// std::cout << RED "Channel destroyed: " RESET << *this <<std::endl;
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
	// std::cout << BLUE "Channel copied: " RESET << *this <<std::endl;
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
		// std::cout << BLUE "Channel assigned: " RESET << *this << std::endl;
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
