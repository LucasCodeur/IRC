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
	try
	{
		Command c0(0, "MODE hi hello there");
		std::cout << c0 << std::endl;
		Command c1(0, "PRIVMSG");
		std::cout << c1 << std::endl;
		Command c2(0, "TOPIC");
		std::cout << c2 << std::endl;
		Command c3(0, "PART bonjour");
		std::cout << c3 << std::endl;
		Command c4(0, "PART");
		std::cout << c4 << std::endl;
		Command c5(0, "");
		std::cout << c5 << std::endl;
		Command c6(0, "none");
		std::cout << c6 << std::endl;
	}
	catch (std::exception &e)
	{
		std::cerr <<" Caught : " << e.what() << std::endl;
	}

	
}
