/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpolizzi <lpolizzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 19:03:07 by lpolizzi          #+#    #+#             */
/*   Updated: 2025/03/03 12:38:24 by lpolizzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/get_next_line.h"

char	*gnl_strdup(const char *s)
{
	char	*dup;
	int		i;

	i = -1;
	dup = malloc(sizeof(char) * (gnl_strlen(s) + 1));
	if (!dup)
		return (NULL);
	while (s[++i])
		dup[i] = s[i];
	dup[i] = '\0';
	return (dup);
}

size_t	gnl_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s && s[i])
		i++;
	return (i);
}

char	*gnl_strjoin(char *s1, const char *s2)
{
	char	*ret;
	int		i;
	int		j;

	if (!s2)
		return (NULL);
	if (!s1)
		return (gnl_strdup(s2));
	ret = malloc(sizeof(char) * (gnl_strlen(s1) + gnl_strlen(s2) + 1));
	if (!ret)
		return (NULL);
	i = -1;
	j = 0;
	while (s1 && s1[++i])
		ret[i] = s1[i];
	while (s2[j])
		ret[i++] = s2[j++];
	ret[i] = '\0';
	free(s1);
	return (ret);
}

int	gnl_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s && s[i])
		if (s[i++] == (char)c)
			return (1);
	return (0);
}
