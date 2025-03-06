/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_hex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 11:40:26 by macorso          #+#    #+#             */
/*   Updated: 2025/03/04 23:04:36 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

static int	ft_hexlen(unsigned int n)
{
	int	len;

	len = 0;
	while (n)
	{
		len++;
		n /= 16;
	}
	return (len);
}

static void	ft_puthex(unsigned int n, const char format, int fd)
{
	if (n >= 16)
	{
		ft_puthex(n / 16, format, fd);
		ft_puthex(n % 16, format, fd);
	}
	else
	{
		if (n <= 9)
			ft_putchar_fd((n + '0'), fd);
		else
		{
			if (format == 'x')
				ft_putchar_fd((n - 10 + 'a'), fd);
			else if (format == 'X')
				ft_putchar_fd((n - 10 + 'A'), fd);
		}
	}
}

int	ft_print_hex(unsigned int n, const char format, int fd)
{
	if (n == 0)
		return (write(fd, "0", 1));
	else
		ft_puthex(n, format, fd);
	return (ft_hexlen(n));
}
