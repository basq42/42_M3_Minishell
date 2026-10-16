/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 13:55:12 by bkelav            #+#    #+#             */
/*   Updated: 2025/05/29 14:55:32 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	if (!(*little))
		return ((char *)big);
	i = 0;
	while (i < len && big[i])
	{
		j = 0;
		while (big[i + j] && little[j]
			&& big[i + j] == little[j] && i + j < len)
		{
			j++;
			if (!(little[j]))
				return ((char *)big + i);
		}
		i++;
	}
	return (NULL);
}
/*
#include <stdio.h>
#include <bsd/string.h>

int	main(int argc, char **argv)
{
	if (argc == 3)
	{
		char	*ptr = ft_strnstr(argv[1], argv[2], 5);
		char	*ptr1 = strnstr(argv[1], argv[2], 5);
		printf("FT: %p\n", ptr);
		printf("OG: %p\n", ptr1);
	}
	return (0);
}*/
