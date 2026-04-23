#include "Client.hpp"

Client::Client()
	: _username(""),
	  _nickname(""),
	  _password(""),
	  _buf(""),
	  _fd(-1),
	  _authState(EMPTY)
{
}

Client::~Client()
{
}

Client::Client(Client const &original)
	: _username(original._username),
	  _nickname(original._nickname),
	  _password(original._password),
	  _buf(original._buf),
	  _fd(original._fd),
	  _authState(original._authState)
{
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
	}
	return *this;
}

std::ostream &operator<<(std::ostream &o, const Client &obj)
{
	(void)obj;
	return o;
}
