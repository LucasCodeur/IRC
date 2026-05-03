/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lud-adam <lud-adam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 14:08:44 by lud-adam          #+#    #+#             */
/*   Updated: 2026/05/03 15:06:35 by lud-adam         ###   ########.fr       */
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
