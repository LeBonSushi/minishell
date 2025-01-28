/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dfeve <dfeve@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:06:11 by dfeve             #+#    #+#             */
/*   Updated: 2025/01/28 01:06:14 by dfeve            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static	is_sep(char *str, char **seplist, int size)
{
	if (ft_strncmp(str, seplist, size) == 0)
		return (1);
	return (-1);
}

int	get_biggest_el(char **seplist)
{
	int	i;
	int	biggest_len;

	i = 0;
	biggest_len = 0;
	while (seplist && seplist[i])
	{
		if (ft_strlen(seplist[i]) > biggest_len)
			biggest_len = ft_strlen(seplist[i]);
		i++;
	}
	return (biggest_len);
}

int	check_sep(char *str, char **seplist)
{
	int	bg_el;

	bg_el = get_biggest_el(seplist);
	while (bg_el > 0)
	{
		if (is_sep(str, seplist, bg_el) == 1)
			return (bg_el);
		bg_el--;
	}
	return (-1);
}

static int	count_words(char *str, char sep, char **seplist)
{
	int	i;
	int	count;
	int	on_sep;

	i = -1;
	count = 0;
	on_sep = 0;
	while (str && str[++i])
	{
		if (str[i] == sep && on_sep == 0)
		{
			count++;
			on_sep = 1;
		}
		else if (is_in_str(str[i], seplist) == 1 && on_sep == 0)
		{
			count += 2;
			on_sep = 1;
		}
		else
			on_sep = 0;
	}
	if (on_sep == 0)
		count++;
	return (count);
}

char	**split_2_sep(const char *str, char sep, char **seplist)
{
	int		i;
	int		on_sep;
	int		y;
	int		start;
	char	**result;

	i = 0;
	y = 0;
	on_sep = 0;
	start = 0;
	result = malloc(count_words(str, sep, seplist) * sizeof(char *));
	while (str && str[i])
	{
		on_sep = 1;
		if (str[i] == sep && on_sep == 0)
			result[y++] = ft_substr(str, start, i - start + 1);
		else if (is_in_str(str[i], seplist) == 1 && on_sep == 0)
		{
			result[y++] = ft_substr(str, start, i - start);
			result[y++] = ft_substr(str, i, i + 1);
		}
		else
		{
			start = i;
			on_sep = 0;
		}
	}
	if (on_sep == 0)
		result[y++] = ft_substr(str, start, i - start);
	return (result);
}
