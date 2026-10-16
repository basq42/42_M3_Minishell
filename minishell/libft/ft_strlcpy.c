/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 17:29:42 by bkelav            #+#    #+#             */
/*   Updated: 2025/06/07 14:35:43 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	srclen;
	size_t	i;

	srclen = ft_strlen(src);
	if (size != 0)
	{
		i = 0;
		while (i < size - 1 && src[i])
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	return (srclen);
}
/*
#include <stdio.h>
#include <bsd/string.h>

int	main(int argc, char **argv)
{
	char	dest[5];
	char	dest1[5];
	char	dest2[5];
	char	dest3[5];

	if (argc == 2)
	{
		printf("FT[5]: %zu\n", ft_strlcpy(dest, argv[1], 5));
		printf("%s\n", dest);
		printf("OG[5]: %zu\n", strlcpy(dest1, argv[1], 5));
		printf("%s\n", dest1);
		printf("FT[10]: %zu\n", ft_strlcpy(dest2, argv[1], 10));
		printf("%s\n", dest2);
		printf("OG[10]: %zu\n", strlcpy(dest3, argv[1], 10));
		printf("%s\n", dest3);
	}
	return (0);
}*/
