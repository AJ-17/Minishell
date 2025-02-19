/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cprojean <cprojean@42lyon.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 17:57:18 by ajakubcz          #+#    #+#             */
/*   Updated: 2023/07/23 00:19:37 by cprojean         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>

int		ft_printf(const char *str, ...);
void	ft_putnbr(int n, ssize_t *count);
void	ft_putunbr(unsigned int n, ssize_t *count);
void	ft_put_hexa(unsigned int nbr, char *base, ssize_t *count);
void	ft_putptr_hexa(size_t nbr, ssize_t *count);
void	ft_putchar(char c, ssize_t *count);
void	ft_putstr(char *str, ssize_t *count);
size_t	after(size_t i, const char *str, va_list va, ssize_t *count);

#endif
