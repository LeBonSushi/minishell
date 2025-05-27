/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 12:10:38 by macorso          #+#    #+#             */
/*   Updated: 2025/03/04 23:05:29 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

int	ft_print_char(const int c, int fd)
{
	write(fd, (unsigned char *)&c, 1);
	return (1);
}

int	ft_formats(va_list args, const char format, int fd)
{
	int	len;

	len = 0;
	if (format == 'c')
		len += ft_print_char(va_arg(args, int), fd);
	else if (format == 's')
		len += ft_print_str(va_arg(args, char *), fd);
	else if (format == 'p')
		len += ft_print_ptr(va_arg(args, void *), fd);
	else if (format == 'd' || format == 'i')
		len += ft_print_nbr(va_arg(args, int), fd);
	else if (format == 'u')
		len += ft_print_unsigned(va_arg(args, unsigned int), fd);
	else if (format == 'x' || format == 'X')
		len += ft_print_hex(va_arg(args, unsigned int), format, fd);
	else if (format == '%')
		len += ft_print_percent(fd);
	return (len);
}

int	ft_fprintf(int fd, const char *str, ...)
{
	int		i;
	va_list	args;
	int		len;

	i = 0;
	len = 0;
	va_start(args, str);
	while (str[i])
	{
		if (str[i] == '%')
		{
			len += ft_formats(args, str[i + 1], fd);
			i++;
		}
		else
			len += write(fd, &str[i], 1);
		i++;
	}
	va_end(args);
	return (len);
}
