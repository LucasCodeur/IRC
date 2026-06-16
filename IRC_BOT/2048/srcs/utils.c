/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lud-adam <lud-adam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 19:39:09 by lud-adam          #+#    #+#             */
/*   Updated: 2026/06/16 19:39:12 by lud-adam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include <stddef.h>

void	*ft_memset(void *pointer, int value, size_t count)
{
	unsigned char	*p;

	p = (unsigned char *)pointer;
	while (count--)
		*p++ = value;
	return (pointer);
}

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

size_t	nbrlen(long long nb)
{
	if (nb == 0)
		return 1;
	size_t len = 0;
	for (int tmp_nb = nb; tmp_nb != 0; tmp_nb /= 10)
		len++;
	return (len);
}

size_t	max(size_t size1, size_t size2)
{
	if (size1 > size2)
		return (size1);
	return (size2);
}
