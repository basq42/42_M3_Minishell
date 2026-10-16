/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 12:56:49 by bkelav            #+#    #+#             */
/*   Updated: 2025/06/07 15:57:54 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
/*
	Overflow check
	Allocation
	fill with 0s
*/
#include <stdint.h>
#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void			*ptr;
	size_t			total;

	if (size != 0 && (nmemb > SIZE_MAX / size))
		return (NULL);
	total = size * nmemb;
	ptr = malloc(total);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, total);
	return (ptr);
}
/*
#include <stdio.h>

int	main(void)
{
	char	*a = ft_calloc(2147483648, sizeof(char));
	char	*b = calloc(2147483648, sizeof(char));
	char	*c = ft_calloc(-2147483649, sizeof(char));
	char	*d = calloc(-2147483649, sizeof(char));

	printf("FT: %s | %p\n", a, &a);
	printf("OG: %s | %p\n", b, &b);
	printf("FT: %s | %p\n", c, &c);
	printf("OG: %s | %p\n", d, &d);
	return 0;
}*/
