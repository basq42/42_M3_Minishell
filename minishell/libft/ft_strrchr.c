/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 15:04:27 by bkelav            #+#    #+#             */
/*   Updated: 2025/06/12 14:29:22 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
/*
char	*ft_strrchr(const char *s, int c)
{
	char	*fin;

	fin = 0;
	if (*s)
	{
		fin = ft_strrchr((s + 1), c);
		if (fin)
			return (fin);
	}
	if (*s == (unsigned char)c)
		return ((char *)s);
	return (0);
}
	recursive may cause issues for long strings?
	also instead of using O(1) stack space it uses O(n)

	in iterative version fin is updated to the last match
*/

#include <stddef.h>

char	*ft_strrchr(const char *s, int c)
{
	const char	*fin;
	char		ch;

	ch = (unsigned char)c;
	fin = NULL;
	while (*s)
	{
		if ((unsigned char)*s == ch)
			fin = s;
		s++;
	}
	if (ch == '\0')
		return ((char *)s);
	return ((char *)fin);
}

/*
#include <stdio.h>
#include <string.h>

int	main(int argc, char **argv)
{
	if (argc == 3)
	{
		char	*ptr = ft_strrchr(argv[1], argv[2][0]);
		char	*ptr1 = strrchr(argv[1], argv[2][0]);
		printf("FT: %p\n", ptr);
		printf("OG: %p\n", ptr1);
	}
	return (0);
}*/
