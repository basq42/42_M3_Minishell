/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 16:25:49 by bkelav            #+#    #+#             */
/*   Updated: 2025/06/14 14:59:31 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/*
   *s = "This Is A String", char c = " " -> s[0] = "This" ....
   function to detect amount of substrings (delimited by c) for malloc
   	- variable count and word_flg (to determine whether in word or not)
   function to duplicate those substrings
   From Libft:
   	- ft_strlen
	maybe use ft_strchr and strdup?
*/

static int	sub_count(char const *s, char c)
{
	int	count;
	int	sub_flg;

	count = 0;
	sub_flg = 0;
	while (*s)
	{
		if (*s != c && sub_flg == 0)
		{
			sub_flg = 1;
			count++;
		}
		else if (*s == c)
			sub_flg = 0;
		s++;
	}
	return (count);
}

static char	*sub_dup(char const *s, int start, int end)
{
	char	*substr;
	int		i;

	if (!s)
		return (NULL);
	i = 0;
	substr = malloc((end - start + 1) * sizeof(*s));
	if (!substr)
		return (NULL);
	while (start < end)
		substr[i++] = s[start++];
	substr[i] = '\0';
	return (substr);
}

static void	split_free(char **s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		free(s[i]);
		i++;
	}
	free(s);
}

static int	split_fill(char **split, const char *s, char c, int indx_flg)
{
	size_t		i;
	size_t		j;

	i = 0;
	j = 0;
	while (i <= ft_strlen(s))
	{
		if (s[i] != c && indx_flg < 0)
			indx_flg = i;
		else if ((s[i] == c || s[i] == '\0') && indx_flg >= 0)
		{
			split[j] = sub_dup(s, indx_flg, i);
			if (!split[j])
			{
				split_free(split);
				return (0);
			}
			j++;
			indx_flg = -1;
		}
		i++;
	}
	split[j] = 0;
	return (1);
}

char	**ft_split(char const *s, char c)
{
	int		subc;
	char	**split;

	if (!s)
		return (NULL);
	subc = sub_count(s, c);
	split = malloc((subc + 1) * sizeof(char *));
	if (!split)
		return (NULL);
	if (!split_fill(split, s, c, -1))
		return (NULL);
	return (split);
}
/*
#include <stdio.h>
int	main(void)
{
	char	s[] = "This String Consists of        Words     delimited by SPACE ";
	char	s1[] = "ThiscStringcConsistscofcccccccWordscdelimitedcbycsmallCc";
	char	s2[] = "This/////String/Consists of////Words///delimited by SLASH/";
	char	d = ' ';
	char	d1 = 'c';
	char	d2 = '/';
	char	**sp = ft_split(s, d);
	char	**sp1 = ft_split(s1, d1);
	char	**sp2 = ft_split(s2, d2);

	int	i = 0;
	printf("SPLIT:");
	while (*sp && sp[i])
	{
		printf(" %s |", sp[i]);
		i++;
	}
	printf("\n");
	i = 0;
	printf("SPLIT1:");
	while (*sp1 && sp1[i])
	{
		printf(" %s |", sp1[i]);
		i++;
	}
	printf("\n");
	i = 0;
	printf("SPLIT2:");
	while (*sp2 && sp2[i])
	{
		printf(" %s |", sp2[i]);
		i++;
	}
	printf("\n");
	split_free(sp);
	split_free(sp1);
	split_free(sp2);
	return (0);
}
*/
/*
#include <stdio.h>

int	main(int argc, char **argv)
{
	if (argc == 3)
	{
		int	i = 0;
		char **s = ft_split(argv[1], argv[2][0]);
		while (s[i])
		{
			printf("%s\n", s[i++]);
		}
	}
	return (0);
}*/
