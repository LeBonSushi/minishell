/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpolizzi <lpolizzi@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 10:34:35 by lpolizzi          #+#    #+#             */
/*   Updated: 2024/11/11 10:36:12 by lpolizzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strstr(const char *haystack, const char *needle)
{
	size_t	i;
	size_t	j;

	if (haystack && needle)
	{
		if (!*needle)
			return ((char *)haystack);
		i = 0;
		while (haystack[i])
		{
			j = 0;
			while (haystack[i + j] == needle[j] && needle[j])
				j++;
			if (!needle[j])
				return ((char *)&haystack[i]);
			i++;
		}
	}
	return (NULL);
}
