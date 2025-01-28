/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dfeve <dfeve@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:06:11 by dfeve             #+#    #+#             */
/*   Updated: 2025/01/28 03:15:12 by dfeve            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*str_to_token(char *str)
{
	char	*result;
	int		i;

	i = 0;
	result = malloc(ft_strlen(str) + 1);
	while (str && str[i])
	{
		result[i] = (char)(get_token(*(str + i)) + '0');
		i++;
	}
	result[i] = 0;
	return (result);
}

static int	split_get_len(char *token_str)
{
	int		i;
	int		count;
	bool	on_inv;

	i = 1;
	count = 1;
	on_inv = false;
	while (token_str && token_str[i])
	{
		if (token_str[i] == '/' && on_inv == false)
		{
			count--;
			on_inv = true;
		}
		if (token_str[i] != token_str[i - 1])
		{
			count++;
			on_inv = false;
		}
		i++;
	}
	return (count);
}

char	**split_str_token(char *str, char *token_str)
{
	char	**result;
	int		i;
	int		y;
	int		start;

	result = malloc((split_get_len(token_str) + 1) * sizeof(char *));
	y = 0;
	i = 1;
	start = 0;
	while (token_str && token_str[i])
	{
		if (token_str[i] != token_str[i - 1])
		{
			result[y++] = ft_substr((const char *)str, start, i - start);
			if (token_str[i] == '/')
				while (token_str[i] == '/')
					i++;
			start = i;
		}
		i++;
	}
	if (str[i] != ' ')
		result[y++] = ft_substr((const char *)str, start, i);
	result[y] = 0;
	return (result);
}

void	print_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab && tab[i])
		printf("%s\n", tab[i++]);
}
