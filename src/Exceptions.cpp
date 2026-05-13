/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lud-adam <lud-adam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 14:08:44 by lud-adam          #+#    #+#             */
/*   Updated: 2026/05/13 10:29:19 by lud-adam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Exceptions.hpp"

const char* socketFailed::what() const throw ()
{
    return ("Socket failed\n");
}

const char* setSocketOptionFailed::what() const throw ()
{
    return ("Set socket option failed\n");
}

const char*  bindFailed::what() const throw ()
{
        return ("Bind failed\n");
};

const char*  listenSocketFailed::what() const throw()
{
    return ("Listen socket failed\n");
}

const char*  epollCreateFailed::what() const throw()
{
    return ("EpollCreate failed\n");
}

const char*  controlEpollFailed::what() const throw()
{
    return ("Epoll_ctl failed\n");
}

const char*  acceptFailed::what() const throw()
{
    return ("Accept failed\n");
}

const char*  epollWaitFailed::what() const throw()
{
    return ("Epoll Wait failed\n");
}

const char*  sendFailed::what() const throw()
{
    return ("Send Failed");
}

const char*  receiveDataFailed::what() const throw()
{
    return ("Receive Data Failed");
}

const char*  setnonblockingFailed::what() const throw()
{
    return ("Setnonblocking Failed");
}

const char* passwordNotCorrect::what() const throw()
{
    return ("Password does not correspond to the server");
}

const char* badCharactersInsidePort::what() const throw()
{
    return ("Characters in port are not correct");
}

const char* badRangePort::what() const throw()
{
    return ("Port bad range");
}
