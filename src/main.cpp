/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enzo <enzo@student.42lyon.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 11:38:11 by lud-adam          #+#    #+#             */
/*   Updated: 2026/04/24 16:52:06 by enzo             ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

// #include "debug.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include <iostream>

int	main(void)
{
	Server server;
	Server server2;
	Channel channel;
	server2 = server;
	std::cout << server2 << std::endl;
	std::cout << server << std::endl;
	std::cout << channel << std::endl;
	return (0);
}
