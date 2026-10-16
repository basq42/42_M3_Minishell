/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:44:28 by bkelav            #+#    #+#             */
/*   Updated: 2025/06/14 15:17:15 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/*
	if(dest>src...) checks whether dest has a higher mem adress
	if yes, copying backwards ensures no corruption in data
	if no, memcpy is sufficient
*/

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;

	if (dest == src || n == 0)
		return (dest);
	d = (unsigned char *)dest;
	s = (const unsigned char *)src;
	if (dest > src && dest < src + n)
	{
		while (n-- > 0)
			d[n] = s[n];
	}
	else
	{
		ft_memcpy(dest, src, n);
	}
	return (dest);
}
/*
#include <stdio.h>
#include <string.h>

int	main(void)
{
	char	s[100] = "Geeksfor";
	char	s1[100] = "Geeksfor";
	char	s2[100] = "Geeksfor";
	char	s3[100] = "Geeksfor";
	char	s4[] = {67, 66, 67, 68, 67};
	char	s5[] = {68, 67, 66, 67, 68};
	int	i[] = {2, 3, 3, 5, 6, 3, 8, 9};
	int	i1[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

	ft_memmove(s + 5, s, 8);
	printf("%s\n", s);
	memmove(s1 + 5, s1, 8);
	printf("%s\n", s1);
	ft_memmove(s3, s2, 8);
	printf("s2: %s | s3: %s\n", s2, s3);
	printf("S4bef: %s\n", s4);
	ft_memmove(s4, s5 + 1, sizeof(s4));
	printf("S4: %s\n", s4);
	ft_memmove(i, i1 + 2, 7);
	int	x = 0;
	while (x <= 7)
	{
		printf("%d", i[x]);
		x++;
	}
	return (0);
}*/
