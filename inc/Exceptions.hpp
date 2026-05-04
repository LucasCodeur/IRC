/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lud-adam <lud-adam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 14:02:20 by lud-adam          #+#    #+#             */
/*   Updated: 2026/05/03 15:06:29 by lud-adam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCEPTIONS_HPP
# define EXCEPTIONS_HPP

#include <exception>
#include <string>

class socketFailed : public std::exception
{
    public:
        const char* what() const throw ();
};

class setSocketOptionFailed : public std::exception
{
    public:
        const char* what() const throw ();
};

class bindFailed: public std::exception
{
    public:
        const char* what() const throw ();
};

class listenSocketFailed: public std::exception
{
    public:
        const char* what() const throw ();
};

class epollCreateFailed: public std::exception
{
    public:
        const char* what() const throw ();
};

class controlEpollFailed: public std::exception
{
    public:
        const char* what() const throw ();
};

class acceptFailed: public std::exception
{
    public:
        const char* what() const throw ();
};


class epollWaitFailed: public std::exception
{
    public:
        const char* what() const throw ();
};

class sendFailed: public std::exception
{
    public:
        const char* what() const throw ();
};


class receiveDataFailed: public std::exception
{
    public:
        const char* what() const throw ();
};

#endif
