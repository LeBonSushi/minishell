/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 23:07:31 by lpolizzi          #+#    #+#             */
/*   Updated: 2025/03/04 23:02:48 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

int	ft_print_str(char *str, int fd)
{
	if (!str)
	{
		ft_putstr_fd("(null)", fd);
		return (6);
	}
	ft_putstr_fd(str, fd);
	return (ft_strlen(str));
}

int	ft_print_nbr(int n, int fd)
{
	char	*num;
	int		len;

	num = ft_itoa(n);
	len = ft_print_str(num, fd);
	free(num);
	return (len);
}

int	ft_print_percent(int fd)
{
	write(fd, "%", 1);
	return (1);
}
