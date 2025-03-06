/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_ptr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 23:55:04 by macorso          #+#    #+#             */
/*   Updated: 2025/03/04 23:03:56 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

static int	ft_ptrlen(uintptr_t ptr)
{
	int	len;

	len = 0;
	while (ptr)
	{
		ptr /= 16;
		len++;
	}
	return (len);
}

static void	ft_putptr(uintptr_t ptr, int fd)
{
	if (ptr >= 16)
	{
		ft_putptr(ptr / 16, fd);
		ft_putptr(ptr % 16, fd);
	}
	else
	{
		if (ptr <= 9)
			ft_putchar_fd((ptr + '0'), fd);
		else
			ft_putchar_fd((ptr - 10 + 'a'), fd);
	}
}

int	ft_print_ptr(void *ptr, int fd)
{
	int			len;
	uintptr_t	address;

	address = (uintptr_t)ptr;
	len = 0;
	if (address == 0)
		len += write(fd, "(nil)", 5);
	else
	{
		len += write(fd, "0x", 2);
		len += ft_ptrlen(address);
		ft_putptr(address, fd);
	}
	return (len);
}
