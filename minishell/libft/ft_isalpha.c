/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 14:35:49 by bkelav            #+#    #+#             */
/*   Updated: 2025/06/12 15:29:36 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* 
1: 	In C logical expressions always return 1 on true and 0 on false
	which is how Boolean logic is represented in C
2: 	2.1: Comp. an int to a char const in C works due to the 'integer promotion'
	rule (built-in). 
	2.2: char type is esentially a small integer type (1byte)
	while int is 4 bytes.
	2.3: When a comparison or arithmatic operation involves both types, char is
	automatically promoted to int
*/
int	ft_isalpha(int c)
{
	return ((c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z'));
}
