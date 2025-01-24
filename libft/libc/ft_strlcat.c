/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 16:16:46 by macorso          #+#    #+#             */
/*   Updated: 2024/10/09 16:19:40 by macorso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *d, const char *s, size_t dstsize)
{
	size_t	dst_len;
	size_t	i;
	size_t	j;

	dst_len = ft_strlen(d);
	j = 0;
	while (d[j])
		j++;
	i = 0;
	while (s[i] && (i + j + 1) < dstsize)
	{
		d[j + i] = s[i];
		i++;
	}
	if (i < dstsize)
		d[j + i] = '\0';
	if (dstsize <= dst_len)
		return (ft_strlen(s) + dstsize);
	else
		return (ft_strlen(s) + dst_len);
}
