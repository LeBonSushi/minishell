/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 22:38:07 by lpolizzi          #+#    #+#             */
/*   Updated: 2025/03/04 22:26:36 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(const char *s1, const char *s2, int freeS1, int freeS2)
{
	char		*ret;
	int			i;
	int			j;
	const char	*empty;

	empty = "";
	i = -1;
	j = 0;
	if (!s1)
		s1 = empty;
	if (!s2)
		s2 = empty;
	ret = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!ret)
		return (NULL);
	while (s1[++i])
		ret[i] = s1[i];
	while (s2[j])
		ret[i++] = s2[j++];
	ret[i] = '\0';
	if (freeS1)
		free((char *)s1);
	if (freeS2)
		free((char *)s2);
	return (ret);
}
