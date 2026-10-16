/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 13:38:31 by bkelav            #+#    #+#             */
/*   Updated: 2025/06/12 14:29:56 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
/*
	for portability typecast to unsigned char
*/
#include <stddef.h>

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t			i;
	unsigned char	*u_s1;
	unsigned char	*u_s2;

	u_s1 = (unsigned char *)s1;
	u_s2 = (unsigned char *)s2;
	i = 0;
	while (i < n)
	{
		if ((u_s1[i] != u_s2[i]) || (u_s1[i] == '\0') || (u_s2[i] == '\0'))
			return (u_s1[i] - u_s2[i]);
		i++;
	}
	return (0);
}
/*
#include <stdio.h>
#include <string.h>

int	main(int argc, char **argv)
{
	if (argc == 3)
	{
		printf("Size = 10\n");
		printf("FT: %d\n", ft_strncmp(argv[1], argv[2], 10));
		printf("OG: %d\n", strncmp(argv[1], argv[2], 10));
	}
	return (0);
}*/
