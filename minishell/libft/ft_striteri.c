/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 15:50:06 by bkelav            #+#    #+#             */
/*   Updated: 2025/06/07 17:36:25 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
/*
	diff from strmapi is this applies f in place(on string itself)
*/
void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	i;

	if (!s || !f)
		return ;
	i = 0;
	while (s[i])
	{
		f(i, &s[i]);
		i++;
	}
}
/*
void	ft_upp(unsigned int i, char *s)
{
	(void)i;
	if (*s >= 'a' && *s <= 'z')
		*s -= 32;
}

#include <stdio.h>

int	main(void)
{
	char s[] = "upp this string";
	ft_striteri(s, ft_upp);
	printf("%s\n", s);
	return (0);
}*/
