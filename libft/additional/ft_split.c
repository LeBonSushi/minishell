/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpolizzi <lpolizzi@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 23:28:02 by lpolizzi          #+#    #+#             */
/*   Updated: 2025/03/03 13:28:38 by lpolizzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_countwords(const char *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] != c && (s[i + 1] == c || s[i + 1] == '\0'))
			count++;
		i++;
	}
	return (count);
}

static void	write_word(char *dest, char *src, char c)
{
	int	i;

	i = 0;
	while (src[i] && src[i] != c)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
}

static void	make_split(char **strs, char *s, char c)
{
	int	i;
	int	j;
	int	word;

	i = 0;
	word = 0;
	while (s[i])
	{
		if (s[i] == c)
			i++;
		else
		{
			j = 0;
			while (s[i + j] != c && s[i + j])
				j++;
			strs[word] = (char *)malloc(sizeof(char) * (j + 1));
			if (!strs[word])
				return ;
			write_word(strs[word], &s[i], c);
			i += j;
			word++;
		}
	}
}

char	**ft_split(const char *s, char c)
{
	char	**strs;

	strs = malloc(sizeof(char *) * (ft_countwords(s, c) + 1));
	if (!strs)
		return (NULL);
	strs[ft_countwords(s, c)] = 0;
	make_split(strs, (char *)s, c);
	return (strs);
}
