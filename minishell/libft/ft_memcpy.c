/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:30:28 by bkelav            #+#    #+#             */
/*   Updated: 2025/06/12 14:46:57 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
/*
	src is const because its read-only
	tester says not protected for Null params
	but thats the og behaviour so...?
*/

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;
	size_t				i;

	d = (unsigned char *)dest;
	s = (const unsigned char *)src;
	i = 0;
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dest);
}
/*
#include <stdio.h>
#include <string.h>

int	main(void)
{
	char	s[] = "Copy This";
	char	s1[5];
	char	s2[] = "Copy This";
	char	s3[5];
	char	s4 = ft_memcpy(s1, NULL, 0);

	ft_memcpy(s1, s, 5);
	printf("s->s1: %s\n", s1);
	memcpy(s3, s2, 5);
	printf("OG s2->s3: %s\n", s3);
	printf("Null->s1: %s\n", s4);
	return (0);
}*/
