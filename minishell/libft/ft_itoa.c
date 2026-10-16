/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 13:52:47 by bkelav            #+#    #+#             */
/*   Updated: 2025/05/28 14:20:33 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
/*
   function to tell how many char needed -> "-123" > '-' '1'...
   	so for n < 0 +1 char for '-'
	for n = 0 -> 1 char
*/

static int	char_amount(int n)
{
	int	amount;

	amount = 0;
	if (n <= 0)
		amount = 1;
	while (n)
	{
		amount++;
		n /= 10;
	}
	return (amount);
}

char	*ft_itoa(int n)
{
	char		*s;
	long		nb;
	int			amount;

	nb = n;
	amount = char_amount(n);
	s = (char *)malloc((amount + 1) * sizeof(char));
	if (!s)
		return (NULL);
	s[amount] = '\0';
	if (nb < 0)
	{
		s[0] = '-';
		nb *= -1;
	}
	if (nb == 0)
		s[0] = '0';
	while (nb > 0)
	{
		amount--;
		s[amount] = (nb % 10) + '0';
		nb /= 10;
	}
	return (s);
}
/*
#include <stdio.h>

int	main(void)
{
	char	*s = ft_itoa(-2147483648);
	char	*s1 = ft_itoa(2147483646);
	char	*s2 = ft_itoa(0);
	char	*s3 = ft_itoa(9876);

	printf("%s\n", s);
	printf("%s\n", s1);
	printf("%s\n", s2);
	printf("%s\n", s3);
	return (0);
}*/
