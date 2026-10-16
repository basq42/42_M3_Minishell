/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:01:46 by bkelav            #+#    #+#             */
/*   Updated: 2025/06/07 16:11:09 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*joind;
	size_t	size;
	size_t	i;
	size_t	j;

	if (!s1 || !s2)
		return (NULL);
	size = ft_strlen(s1) + ft_strlen(s2);
	joind = (char *)malloc(size + 1);
	if (!joind)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		joind[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
	{
		joind[i + j] = s2[j];
		j++;
	}
	joind[i + j] = '\0';
	return (joind);
}
/*
#include <stdio.h>

int	main(void)
{
	char	s[] = "Prefix";
	char	s1[] = "Suffix";
	char	*t = ft_strjoin(s, s1);
	printf("%s\n", t);
	return 0;
}*/
