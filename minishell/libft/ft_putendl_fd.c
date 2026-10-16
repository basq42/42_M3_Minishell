/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkelav <bkelav@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 12:35:45 by bkelav            #+#    #+#             */
/*   Updated: 2025/05/29 12:41:05 by bkelav           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"
/*
void	ft_putchar_fd(char c, int fd);

void	ft_putstr_fd(char *s, int fd);
*/

void	ft_putendl_fd(char *s, int fd)
{
	if (!s)
		return ;
	ft_putstr_fd(s, fd);
	ft_putchar_fd('\n', fd);
}
/*
int	main(void)
{
	ft_putendl_fd("Output This", 1);
	ft_putendl_fd("And This", 1);
	return (0);
}*/
