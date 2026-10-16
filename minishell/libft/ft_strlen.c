/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 15:52:03 by bkelav            #+#    #+#             */
/*   Updated: 2025/06/07 13:03:58 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
/*
	const char because funct. point is to read, not alter
*/
size_t	ft_strlen(const char *s)
{
	const char	*p;

	p = s;
	while (*p)
		p++;
	return (p - s);
}
/*
#include <string.h>
#include <stdio.h>

int	main(int argc, char **argv)
{
	if (argc == 2)
	{
		printf("FT: %lu\n", ft_strlen(argv[1]));
		printf("OG: %lu\n", strlen(argv[1]));
	}
	return (0);
}*/
