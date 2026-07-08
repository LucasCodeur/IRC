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
	  _mode(4),
	  _userLimit(0)
{
	if (DEBUG == 1)
		std::cout << DBUG GREEN "Channel created: " RESET << *this <<std::endl;
}

Channel::Channel(std::string const &name)
	: _name(name),
	  _topic(""),
	  _password(""),
	  _mode(4),
	  _userLimit(0)
{
	if (DEBUG == 1)
		std::cout << DBUG GREEN "Channel created: " RESET << *this <<std::endl;
}

Channel::Channel(std::string const &name, std::string const &password)
	: _name(name),
	  _topic(""),
	  _password(password),
	  _mode(4),
	  _userLimit(0)
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
	  _mode(original._mode),
	  _userLimit(original._userLimit)
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
		this->_userLimit = other._userLimit;
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

std::vector<Client *> const &Channel::getUsers() const
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

size_t Channel::getUserLimit() const
{
	return (this->_userLimit);
}

int Channel::isChanOp(std::string userName) const
{
	int fd = this->getClient(userName)->getFd();
	for (size_t i = 0; i < this->_operators.size(); ++i)
	{
		if (this->_operators[i] == fd)
			return (1);
	}
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

void Channel::setUserLimit(size_t limit)
{
	this->_userLimit = limit;
}

/**
 * @brief Adds a user to the channel if they are not already present. Sends a JOIN message to all other users in the channel.
 * @param client pointer to the Client object to add to the channel.
 * @return true if the user was successfully added, false if the user was already in the channel.
 */
bool Channel::addUser(Client *client)
{
	// Client chanBot = this->getClient(BOT_NAME);
	//
	// commandFactory::createCommand
	if (std::find(this->_users.begin(), this->_users.end(), client) == this->_users.end())
	{
		this->_users.push_back(client);
		return (true);
	}
	return (false);
}
/**
 * @brief removes the specified client from the Channel.
 * @param client pointer to the Client object to remove from the channel.
 * @return true if the user was successfully removed, false if the user was not in the channel.
 */
bool Channel::removeUser(Client *client)
{
	std::vector<Client *>::iterator it = std::find(this->_users.begin(), this->_users.end(), client);
	if (it != this->_users.end())
	{
		this->_users.erase(it);
		return (true);
	}
	return (false);
}

/**
 * @brief removes the specified client from the Channel.
 * @param clientFd fd of the client to remove.
 * @return true if the user was successfully removed, false if the user was not in the channel.
 */
bool Channel::removeUser(int clientFd)
{
	std::vector<Client *>::iterator it;
	for (it = this->_users.begin(); it != this->_users.end(); ++it)
	{
		if ((*it)->getFd() == clientFd)
		{
			this->_users.erase(it);
			return (true);
		}
	}
	return (false);
}

/**
 * @brief removes the specified client from the Channel.
 * @param clientFd nickname of the user to remove.
 * @return true if the user was successfully removed, false if the user was not in the channel.
 */
bool Channel::removeUser(std::string nickname)
{
	std::vector<Client *>::iterator it;
	for (it = this->_users.begin(); it != this->_users.end(); ++it)
	{
		if ((*it)->getNickname() == nickname)
		{
			this->_users.erase(it);
			return (true);
		}
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
	for (std::vector<Client *>::const_iterator it = this->_users.begin(); it != this->_users.end(); ++it)
	{
		if ((*it)->getFd() == clientFd)
			return (true);
	}
	return (false);
}

bool Channel::isOnChan(std::string nickname)
{
	for (std::vector<Client *>::const_iterator it = this->_users.begin(); it != this->_users.end(); ++it)
	{
		if ((*it)->getNickname() == nickname)
			return (true);
	}
	return (false);
}

void Channel::sendMessageToAll(const std::string &message) const
{
	for (std::vector<Client *>::const_iterator it = this->_users.begin(); it != this->_users.end(); ++it)
	{
		(*it)->addToBuffer(message);
	}
}

void Channel::sendMessageToAllOther(const std::string &message, int senderFd) const
{
	for (std::vector<Client *>::const_iterator it = this->_users.begin(); it != this->_users.end(); ++it)
	{
		if ((*it)->getFd() != senderFd)
			(*it)->addToBuffer(message);
	}
}

bool Channel::isUserInChannel(int clientFd) const
{
	for (size_t i = 0; i < this->_users.size(); i++)
		if (this->_users[i]->getFd() == clientFd) return true;
	return false;
}

std::vector<std::string> Channel::listNames()
{
	std::vector<std::string> nicknamesList;
	for (std::vector<Client *>::iterator it = this->_users.begin(); it != this->_users.end(); ++it)
	{
		if (isChanOp((*it)->getNickname()))
			nicknamesList.push_back("@" + (*it)->getNickname());
		else
			nicknamesList.push_back((*it)->getNickname());
	}
	return nicknamesList;
}

std::string Channel::getChannelNamesList() const
{
	std::string namesList;
	std::vector<Client *> const &users = this->getUsers();
	for (size_t i = 0; i < users.size(); i++)
	{
		if (!namesList.empty()) namesList += " ";
		if (this->isOp(users[i]->getFd()))
			namesList += "@";
		namesList += users[i]->getNickname();
	}
	return namesList;
}

void Channel::addInvite(int fd)
{
	if (std::find(this->_invited.begin(), this->_invited.end(), fd) == this->_invited.end())
		this->_invited.push_back(fd);
}

bool Channel::isInvited(int fd) const
{
	return (std::find(this->_invited.begin(), this->_invited.end(), fd) != this->_invited.end());
}

void Channel::removeInvite(int fd)
{
	std::vector<int>::iterator it = std::find(this->_invited.begin(), this->_invited.end(), fd);
	if (it != this->_invited.end())
		this->_invited.erase(it);
}

Client *Channel::getClient(const int fd) const
{
	for (std::vector<Client *>::const_iterator it = this->_users.begin(); it != this->_users.end(); ++it)
	{
		if ((*it)->getFd() == fd)
			return (*it);
	}
	return (NULL);
}

Client *Channel::getClient(const std::string nickname) const
{
	for (std::vector<Client *>::const_iterator it = this->_users.begin(); it != this->_users.end(); ++it)
	{
		if ((*it)->getNickname() == nickname)
			return (*it);
	}
	return (NULL);
}
