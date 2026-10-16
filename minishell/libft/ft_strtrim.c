/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:25:02 by bkelav            #+#    #+#             */
/*   Updated: 2025/06/07 16:12:52 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/*
   ft_strchr
   ft_strlcpy
ex: s1[] = "addadadTrimmed ad String adadadad"
set[] = "ad"
trim[] = "Trimmed ad String"
*/

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;
	char	*trimd;

	if (!s1 || !set)
		return (NULL);
	start = 0;
	end = ft_strlen(s1);
	while (s1[start] && ft_strchr(set, s1[start]))
		start++;
	while (end > start && ft_strchr(set, s1[end - 1]))
		end--;
	trimd = (char *)malloc(end - start + 1);
	if (!trimd)
		return (NULL);
	ft_strlcpy(trimd, (s1 + start), (end - start + 1));
	return (trimd);
}
/*
#include <stdio.h>

int	main(void)
{
	char	s[] = "adddadadadddadaTrimmed But ad Staysddadadadadad";
	char	set[] = "ad";
	char	*t = ft_strtrim(s, set);
	printf("%s\n", t);
	return (0);
}*/
