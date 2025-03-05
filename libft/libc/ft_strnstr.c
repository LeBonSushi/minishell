/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpolizzi <lpolizzi@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 15:20:53 by lpolizzi          #+#    #+#             */
/*   Updated: 2025/03/03 12:35:25 by lpolizzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;

	if (haystack && needle)
	{
		if (!*needle)
			return ((char *)haystack);
		i = 0;
		while (haystack[i] && i < len)
		{
			j = 0;
			while (haystack[i + j] == needle[j] && haystack[i + j] && needle[j]
				&& i + j < len)
				j++;
			if (!needle[j])
				return ((char *)&haystack[i]);
			i++;
		}
	}
	return (NULL);
}
