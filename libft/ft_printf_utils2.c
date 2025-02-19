/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cprojean <cprojean@42lyon.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 11:06:00 by ajakubcz          #+#    #+#             */
/*   Updated: 2023/07/18 11:21:32 by cprojean         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_putchar(char c, ssize_t *count)
{
	if (*count == -1)
		return ;
	if (write(1, &c, 1) == -1)
		*count = -1;
	else
		*count = *count + 1;
}

void	ft_putstr(char *str, ssize_t *count)
{
	size_t	i;

	i = 0;
	if (!str)
	{
		ft_putstr("(null)", count);
		return ;
	}
	while (str[i])
	{
		ft_putchar(str[i], count);
		i++;
	}
}
