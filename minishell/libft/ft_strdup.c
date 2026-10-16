/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 13:49:31 by bkelav            #+#    #+#             */
/*   Updated: 2025/06/07 16:03:26 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
/*
	size_t is better for portability and large arrays
*/
#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*d;
	size_t	s_len;
	size_t	i;

	s_len = ft_strlen(s);
	d = (char *)malloc(s_len + 1);
	if (!d)
		return (NULL);
	i = 0;
	while (s[i])
	{
		d[i] = s[i];
		i++;
	}
	d[i] = '\0';
	return (d);
}
/*
#include <stdio.h>
#include <string.h>

int	main(void)
{
	char	s[] = "Duplicate This";
	char	s1[] = "Duplicate This";
	char	s2[10];
	char	s3[10];
	char	*t = ft_strdup(s);
	char	*t1 = strdup(s1);
	char	*t2 = ft_strdup(s2);
	char	*t3 = strdup(s3);

	printf("FT: %s\n", t);
	printf("OG: %s\n", t1);
	printf("FT: %s\n", t2);
	printf("OG: %s\n", t3);
	return (0);
}*/
