/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 16:56:35 by bkelav            #+#    #+#             */
/*   Updated: 2025/06/12 15:54:11 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	ft_memset(s, 0, n);
}
/*#include <stddef.h>

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*ptr;
	size_t			i;

	ptr = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		ptr[i] = 0;
		i++;
	}
}*/
/*
#include <stdio.h>
#include <strings.h>

int	main(void)
{
	char	a[] = "Try Putting Zeros";
	char	b[] = "Try Putting Zeros";

	printf("FT:\n");
	printf("before: %s | ", a);
	ft_bzero(a + 3, 6);
	printf("after: %s\n", a);
	printf("OG:\n");
	printf("before: %s | ", b);
	bzero(b + 3, 6);
	printf("after: %s\n", b);
	return (0);
}*/
