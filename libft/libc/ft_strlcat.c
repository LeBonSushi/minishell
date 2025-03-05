/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpolizzi <lpolizzi@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 16:16:46 by lpolizzi          #+#    #+#             */
/*   Updated: 2025/03/03 11:12:10 by lpolizzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *d, const char *s, size_t dstsize)
{
	size_t	dst_len;
	size_t	src_len;
	size_t	i;

	dst_len = ft_strlen(d);
	src_len = ft_strlen(s);
	if (dstsize <= dst_len)
		return (dstsize + src_len);
	i = 0;
	while (s[i] && (dst_len + i + 1) < dstsize)
	{
		d[dst_len + i] = s[i];
		i++;
	}
	d[dst_len + i] = '\0';
	return (dst_len + src_len);
}
