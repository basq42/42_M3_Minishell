/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 15:58:43 by bkelav            #+#    #+#             */
/*   Updated: 2025/06/07 12:34:18 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*ptr;

	ptr = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		ptr[i] = (unsigned char)c;
		i++;
	}
	return (s);
}
/*
#include <stdio.h>
#include <string.h>

int	main(void)
{
	char	a[] = "Fill This With Dots";
	char	a1[] = "Fill This With Dots";
	char	b[10] = {0};
	char	b1[10] = {0};

	printf("FT:\n");
	printf("a : %s | ", a);
	char	*n = (char *)ft_memset(a, '.', 15);
	printf("After: %s\n", n);
	printf("OG:\n");
	printf("a1: %s | ", a1);
	char	*n1 = memset(a1, '.', 15);
	printf("After: %s\na done\n", n1);
	printf("FT:\n");
	printf("b : %s | ", b);
	char	*n2 = ft_memset(b, 1, 10);
	printf("After: %s\n", n2);
	printf("OG:\n");
	printf("b1: %s | ", b1);
	char	*n3 = memset(b1, 1, 10);
	printf("After: %s\nb done", n3);
	return (0);
}*/
