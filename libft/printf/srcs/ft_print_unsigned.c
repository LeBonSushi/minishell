/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_unsigned.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 23:19:38 by lpolizzi          #+#    #+#             */
/*   Updated: 2025/03/04 23:03:15 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

static int	ft_numlen(unsigned int n)
{
	int	len;

	len = 0;
	while (n)
	{
		n /= 10;
		len++;
	}
	return (len);
}

static char	*ft_uitoa(unsigned int n)
{
	char	*ret;
	int		len;

	if (!n)
		return (NULL);
	len = ft_numlen(n);
	ret = malloc(sizeof(char) * (len + 1));
	if (!ret)
		return (NULL);
	ret[len] = 0;
	while (n)
	{
		ret[len - 1] = (n % 10) + '0';
		n /= 10;
		len--;
	}
	return (ret);
}

int	ft_print_unsigned(unsigned int n, int fd)
{
	int		len;
	char	*num;

	if (n == 0)
		return (write(fd, "0", 1));
	else
		num = ft_uitoa(n);
	len = ft_print_str(num, fd);
	free(num);
	return (len);
}
