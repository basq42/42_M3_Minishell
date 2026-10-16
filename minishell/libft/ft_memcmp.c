/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:39:27 by bkelav            #+#    #+#             */
/*   Updated: 2025/06/07 15:46:35 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*u_s1;
	const unsigned char	*u_s2;
	size_t				i;

	u_s1 = (const unsigned char *)s1;
	u_s2 = (const unsigned char *)s2;
	i = 0;
	while (i < n)
	{
		if (u_s1[i] != u_s2[i])
			return (u_s1[i] - u_s2[i]);
		i++;
	}
	return (0);
}
/*
#include <stdio.h>
#include <string.h>
int	main(void)
{
	char	s[] = "Comparing This";
	char	s1[] = "Comparing That";

	printf("FT: %d\n", ft_memcmp(s, s1, 17));
	printf("OG: %d\n", memcmp(s, s1, 17));
	return (0);
}*/
