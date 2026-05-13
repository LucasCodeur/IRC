#include "Client.hpp"
#include "debug.hpp"
#include <iostream>
#include <string>

Client::Client()
	: _username(""),
	  _nickname(""),
	  _password(""),
	  _buf(""),
	  _fd(-1),
	  _authState(EMPTY)
{
	if (DEBUG == 1)
		std::cout << DBUG GREEN "Client created: " RESET << *this <<std::endl;
}

Client::~Client()
{
	if (DEBUG == 1)
		std::cout << DBUG RED "Client destroyed: " RESET << *this <<std::endl;
}

Client::Client(Client const &original)
	: _username(original._username),
	  _nickname(original._nickname),
	  _password(original._password),
	  _buf(original._buf),
	  _fd(original._fd),
	  _authState(original._authState)
{
	if (DEBUG == 1)
		std::cout << DBUG BLUE "Client copied: " RESET << *this <<std::endl;
}

Client &Client::operator=(Client const &other)
{
	if (this != &other)
	{
		this->_username = other._username;
		this->_nickname = other._nickname;
		this->_password = other._password;
		this->_buf = other._buf;
		this->_fd = other._fd;
		this->_authState = other._authState;
		if (DEBUG == 1)
			std::cout << DBUG BLUE "Client assigned: " RESET << *this << std::endl;
	}
	return (*this);
}

const char *Client::authStateToString(Client::authState state)
{
	switch (state)
	{
		case EMPTY:
		{
			return ("Empty");
		}
		case PASSWORD_RECEIVED:
		{
			return ("Password received");
		}
		case NICK_RECEIVED:
		{
			return ("Nick received");
		}
		case FULLY_REGISTERED:
		{
			return ("Fully registered");
		}
	}
	return ("Unknown");
}

std::ostream &operator<<(std::ostream &o, const Client &obj)
{
	Client::authState currentState = obj.getAuthState();

	return (o << "Client: " << obj.getNickname()
			  << " (Username: " << obj.getUsername()
			  << ", FD: " << obj.getFd()
			  << ", AuthState: " << Client::authStateToString(currentState)
			  << "(" << currentState << ")"
			  << ")");
}

std::string const &Client::getUsername() const
{
	return (_username);
}
std::string const &Client::getNickname() const
{
	return (_nickname);
}

std::string const &Client::getPassword() const
{
	return (_password);
}

std::string const &Client::getBuf() const
{
	return (_buf);
}

int Client::getFd() const
{
	return (_fd);
}

Client::authState Client::getAuthState() const
{
	return (_authState);
}

void Client::setFd(int fd)
{
	this->_fd = fd;
}

void Client::setUsername(std::string const &username)
{
	this->_username = username;
}

void Client::setNickname(std::string const &nickname)
{
	this->_nickname = nickname;
}

void Client::setPassword(std::string const &password)
{
	this->_password = password;
}

void Client::setAuthState(Client::authState state)
{
	this->_authState = state;
}
