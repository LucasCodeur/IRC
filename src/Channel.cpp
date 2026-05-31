#include "Channel.hpp"
#include <algorithm>
#include "debug.hpp"
#include <iostream>
#include <string>
#include <ostream>
#include <sys/socket.h>
#include <sys/types.h>

Channel::Channel()
	: _name(""),
	  _topic(""),
	  _password(""),
	  _mode(0100)
{
	if (DEBUG == 1)
		std::cout << DBUG GREEN "Channel created: " RESET << *this <<std::endl;
	std::cout << " with mode " << _mode << std::endl;
}

Channel::Channel(std::string const &name)
	: _name(name),
	  _topic(""),
	  _password(""),
	  _mode(0100)
{
	if (DEBUG == 1)
		std::cout << DBUG GREEN "Channel created: " RESET << *this <<std::endl;
}

Channel::Channel(std::string const &name, std::string const &password)
	: _name(name),
	  _topic(""),
	  _password(password),
	  _mode(0)
{
	if (DEBUG == 1)
		std::cout << DBUG GREEN "Channel created: " RESET << *this <<std::endl;
}

Channel::~Channel()
{
	for (size_t i = 0; i < _users.size(); ++i)
	{
		if (DEBUG == 1)
			std::cout << DBUG YELLOW "Removing user " RESET << _users[i] << " from channel " << _name << std::endl;
	}
	if (DEBUG == 1)
		std::cout << DBUG RED "Channel destroyed: " RESET << *this <<std::endl;
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
	if (DEBUG == 1)
		std::cout << DBUG BLUE "Channel copied: " RESET << *this <<std::endl;
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
		if (DEBUG == 1)
			std::cout << DBUG BLUE "Channel assigned: " RESET << *this << std::endl;
	}
	return (*this);
}

std::ostream &operator<<(std::ostream &o, const Channel &obj)
{
	return (o << "Channel: " << obj.getName()
			  << " Topic: " << obj.getTopic()
			  << " Password: '" << obj.getPassword() << "'"
			  << ", Users: " << obj.getUsers().size()
			  << ", Operators: " << obj.getOperators().size()
			  << ", Invited: " << obj.getInvited().size() << ")");
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

int Channel::getMode(int i) const
{
	return this->_mode[i];
}

int	Channel::isInviteOnly() const
{
	std::cout << "Invite only mode: " << this->_mode[i] << std::endl;
	return (this->_mode[i]);
}

int Channel::hasPassword() const
{
	return (this->_mode[k]);
}

int Channel::isTopicRestricted() const
{
	return (this->_mode[t]);
}

int Channel::hasUserLimit() const
{
	return (this->_mode[l]);
}

int Channel::isChanOp(std::string userName) const
{
	//TODO: need chanops to be listed by name in Channel to do this
	(void)userName;
	return (0);
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

void Channel::setModeItem(unsigned int item, bool value)
{
	if (item >= _mode.size())
		return; // Invalid item index, do nothing
	this->_mode.set(item, value);
}

/**
 * @brief Adds a user to the channel if they are not already present. Sends a JOIN message to all other users in the channel.
 * @param clientFd the file descriptor of the user to add.
 * @return true if the user was successfully added, false if the user was already in the channel.
 */
bool Channel::addUser(int clientFd)
{
	if (std::find(this->_users.begin(), this->_users.end(), clientFd) == this->_users.end())
	{
		this->_users.push_back(clientFd);
		return (true);
	}
	return (false);
}

bool Channel::removeUser(int clientFd)
{
	std::vector<int>::iterator it = std::find(this->_users.begin(), this->_users.end(), clientFd);
	if (it != this->_users.end())
	{
		this->_users.erase(it);
		return (true);
	}
	return (false);
}

bool Channel::setOperator(int clientFd)
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

bool Channel::isOp(int clientFd) const
{
	return (std::find(this->_operators.begin(), this->_operators.end(), clientFd) != this->_operators.end());
}

bool Channel::isOnChan(int clientFd)
{
	return (std::find(this->_users.begin(), this->_users.end(), clientFd) != this->_users.end());
}

void Channel::sendMessageToAll(const std::string &message) const
{
	for (size_t i = 0; i < this->_users.size(); ++i)
	{
		send(this->_users[i], message.c_str(), message.size(), 0);
	}
}

void Channel::sendMessageToAllOther(const std::string &message, int senderFd) const
{
	for (size_t i = 0; i < this->_users.size(); ++i)
	{
		if (this->_users[i] != senderFd)
			send(this->_users[i], message.c_str(), message.size(), 0);
	}
}

bool Channel::isUserInChannel(int clientFd) const
{
	return (std::find(this->_users.begin(), this->_users.end(), clientFd) != this->_users.end());
}
