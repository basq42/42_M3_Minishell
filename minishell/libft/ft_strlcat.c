/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 17:48:34 by bkelav            #+#    #+#             */
/*   Updated: 2025/06/12 14:28:44 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
/*
	dstlen manually to not go over size
	srclen from ft_strlen as its read only so its safe
*/
#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	dstlen;
	size_t	srclen;
	size_t	i;

	dstlen = 0;
	while (dstlen < size && dst[dstlen])
		dstlen++;
	srclen = ft_strlen(src);
	if (dstlen == size)
		return (size + srclen);
	i = 0;
	while ((dstlen + 1 + i) < size && src[i])
	{
		dst[dstlen + i] = src[i];
		i++;
	}
	if (dstlen + i < size)
		dst[dstlen + i] = '\0';
	return (dstlen + srclen);
}
/*
#include <stdio.h>
#include <bsd/string.h>

int	main(int argc, char **argv)
{
	char	dest[15] = {0};
	char	dest1[15] = {0};
	char	dest2[15] = "0123456789";
	char	dest3[15] = "0123456789";

	if (argc == 2)
	{
		printf("FT[14]: %zu | %s\n", ft_strlcat(dest, argv[1], 14), dest);
		printf("OG[14]: %zu | %s\n", strlcat(dest1, argv[1], 14), dest1);
		printf("FT[5+10]: %zu | %s\n", ft_strlcat(dest2, argv[1], 10), dest2);
		printf("OG[5+10]: %zu | %s\n", strlcat(dest3, argv[1], 10), dest3);
	}
	return (0);
}*/
