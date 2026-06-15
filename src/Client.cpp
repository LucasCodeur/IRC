#include "Client.hpp"
#include "debug.hpp"
#include <iostream>
#include <string>

Client::Client()
	: _authstate(),
	  _username(""),
	  _hostname(""),
	  _realname(""),
	  _servername(""),
	  _nickname(""),
	  _password(""),
	  _buf(""),
	  _fd(-1)
{
	if (DEBUG == 1)
		std::cout << DBUG GREEN "Client created: " RESET << *this <<std::endl;
}

Client::~Client()
{
	if (DEBUG == 1)
		std::cout << DBUG RED "Client destroyed: " RESET << *this <<std::endl;
}

Client::Client(Client const &original) :
	  _authstate(original._authstate),
	  _username(original._username),
	  _hostname(original._hostname),
	  _realname(original._realname),
	  _servername(original._servername),
	  _nickname(original._nickname),
	  _password(original._password),
	  _buf(original._buf),
	  _fd(original._fd)
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
		this->_realname = other._realname;
		this->_servername = other._servername;
		this->_password = other._password;
		this->_buf = other._buf;
		this->_fd = other._fd;
		this->_authstate = other._authstate;
		if (DEBUG == 1)
			std::cout << DBUG BLUE "Client assigned: " RESET << *this << std::endl;
	}
	return (*this);
}

std::ostream &operator<<(std::ostream &o, const Client &obj)
{
	// Client::Authstate currentState = obj.Authstate;

	return (o << "Client: " << obj.getNickname()
			  << " (Username: " << obj.getUsername()
			  << ", FD: " << obj.getFd()
			  // << ", Authstate: " << Client::AuthstateToString(currentState)
			  // << "(" << currentState << ")"
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

std::string &Client::getBuf()
{
	return (_buf);
}

int Client::getFd() const
{
	return (_fd);
}

std::string const &Client::getHostname() const
{
	return (this->_hostname);
}

std::string const &Client::getRealname() const
{
	return (this->_realname);
}

std::string const &Client::getServername() const
{
	return (this->_servername);
}

Authstate&	Client::getAuthstate()
{
	return (this->_authstate);
}

std::string &Client::getClientInputBuffer()
{
	return (this->_inputBuffer);
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

void Client::setRealname(std::string const &realname)
{
	this->_realname = realname;
}


void Client::setHostname(std::string const &hostname)
{
	this->_hostname = hostname;
}

Authstate::Authstate()
{
	this->nickReceived = false;
	this->passwordReceived = false;
	this->fullyRegistered = false;
}

Authstate::Authstate(bool statePassword) :
	passwordReceived(statePassword),
	nickReceived(false),
	fullyRegistered(false)
{}

Authstate::Authstate(const Authstate& other)
{
	if (this != &other)
	{
		this->passwordReceived = other.passwordReceived;
		this->nickReceived = other.nickReceived;
		this->fullyRegistered = other.fullyRegistered;
	}
}

bool	Authstate::getPasswordReceived()
{
	return (this->passwordReceived);
}

bool	Authstate::getNickReceived()
{
	return (this->nickReceived);
}

bool	Authstate::getFullyRegistered()
{
	return (this->fullyRegistered);
}

void	Authstate::setPasswordReceived(bool state)
{
	this->passwordReceived = state;
}

void	Authstate::setNickReceived(bool state)
{
	this->nickReceived = state;
}

void	Authstate::setFullyRegistered(bool state)
{
	this->fullyRegistered = state;
}

void Client::addToBuffer(std::string data)
{
	this->_inputBuffer += data;
}
