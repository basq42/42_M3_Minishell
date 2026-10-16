/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 14:44:24 by bkelav            #+#    #+#             */
/*   Updated: 2025/06/07 14:50:48 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
/*
	c being int allows passing an value that can fit uns char
	if dont type cast (unsigned char)c, chars with vals above 127
	or negative values can create issues
*/
#include <stddef.h>

char	*ft_strchr(const char *s, int c)
{
	unsigned int	i;
	unsigned char	ch;

	i = 0;
	ch = (unsigned char)c;
	while (*s)
	{
		if ((unsigned char)*s == ch)
			return ((char *)s);
		s++;
	}
	if (ch == '\0')
		return ((char *)s + i);
	return (NULL);
}
/*
#include <stdio.h>
#include <string.h>

int	main(int argc, char **argv)
{
	if (argc == 3)
	{
		char	*ptr = ft_strchr(argv[1], argv[2][0]);
		char	*ptr1 = strchr(argv[1], argv[2][0]);
		printf("FT: %p\n", ptr);
		printf("OG: %p\n", ptr1);
	}
	return (0);
}*/
