/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enzo <enzo@student.42lyon.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 10:14:44 by lud-adam          #+#    #+#             */
/*   Updated: 2026/04/24 16:48:13 by enzo             ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <iostream>

#define RESET     "\033[0m"
#define RED       "\033[31m"
#define GREEN     "\033[32m"
#define YELLOW    "\033[33m"
#define BLUE      "\033[34m"
#define MAGENTA   "\033[35m"
#define CYAN      "\033[36m"
#define WHITE     "\033[37m"

#define PRINT(x, color, newline) \
    std::cout << color << x <<newline;

#endif // !DEBUG_HPP
