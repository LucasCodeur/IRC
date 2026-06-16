/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lud-adam <lud-adam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 19:56:04 by lud-adam          #+#    #+#             */
/*   Updated: 2026/06/16 19:56:06 by lud-adam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H
# include <stddef.h>

void	*ft_memset(void *pointer, int value, size_t count);
size_t	ft_strlen(const char *str);
size_t	nbrlen(long long nb);
size_t	max(size_t size1, size_t size2);

#endif
