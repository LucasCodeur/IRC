/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lud-adam <lud-adam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 14:08:10 by lud-adam          #+#    #+#             */
/*   Updated: 2026/05/06 14:08:13 by lud-adam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "debug.hpp"
#include <iostream>

Client::Client()
	: _username(""),
	  _nickname(""),
	  _password(""),
	  _buf(""),
	  _fd(-1),
	  _authState(EMPTY)
{
	std::cout << GREEN "Client created: " RESET << *this <<std::endl;
}

Client::~Client()
{
	std::cout << RED "Client destroyed: " RESET << *this <<std::endl;
}

Client::Client(Client const &original)
	: _username(original._username),
	  _nickname(original._nickname),
	  _password(original._password),
	  _buf(original._buf),
	  _fd(original._fd),
	  _authState(original._authState)
{
	std::cout << BLUE "Client copied: " RESET << *this <<std::endl;
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
		std::cout << BLUE "Client assigned: " RESET << *this << std::endl;
	}
	return (*this);
}

std::ostream &operator<<(std::ostream &o, const Client &obj)
{
	return (o << "Client: " << obj.getNickname() 
			  << " (Username: " << obj.getUsername() 
			  << ", FD: " << obj.getFd() 
			  << ", AuthState: " << obj.getAuthState()
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
