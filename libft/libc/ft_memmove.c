/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpolizzi <lpolizzi@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 15:10:09 by lpolizzi          #+#    #+#             */
/*   Updated: 2025/03/03 12:28:04 by lpolizzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char	*dest;
	unsigned char	*source;

	if (!dst || !src)
		return (NULL);
	dest = (unsigned char *)dst;
	source = (unsigned char *)src;
	if (dest < source)
		while (len--)
			*dest++ = *source++;
	else
	{
		dest += len;
		source += len;
		while (len--)
			*(--dest) = *(--source);
	}
	return (dst);
}
