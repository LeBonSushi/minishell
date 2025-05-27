/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   continuation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 22:38:17 by macorso           #+#    #+#             */
/*   Updated: 2025/05/27 16:46:55 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_quote_continuation(char *line)
{
	int		i;
	int		in_single;
	int		in_double;

	i = 0;
	in_single = 0;
	in_double = 0;
	while (line[i])
	{
		if (line[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (line[i] == '"' && !in_single)
			in_double = !in_double;
		i++;
	}
	return (in_single || in_double);
}

int	is_redirection_before_pipe(char *line)
{
	int		i;
	int		in_single;
	int		in_double;

	i = 0;
	in_single = 0;
	in_double = 0;
	while (line[i])
	{
		if (line[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (line[i] == '"' && !in_single)
			in_double = !in_double;
		else if (!in_single && !in_double)
		{
			if ((line[i] == '>' || line[i] == '<')
				&& line[i + 1] && line[i + 1] == '|')
				return (1);
			if (line[i] == '>' && line[i + 1] && line[i + 1] == '>'
				&& line[i + 2] && line[i + 2] == '|')
				return (1);
		}
		i++;
	}
	return (0);
}

int	has_incomplete_redirection(char *line)
{
	int	i;
	int	len;

	len = ft_strlen(line);
	if (len < 2)
		return (0);
	i = len - 1;
	while (i >= 0 && ft_isspace(line[i]))
		i--;
	if (i >= 0 && line[i] == '|')
	{
		i--;
		while (i >= 0 && ft_isspace(line[i]))
			i--;
		if (i >= 1 && line[i] == '>' && line[i - 1] == '>')
			return (1);
		if (i >= 0 && (line[i] == '>' || line[i] == '<'))
			return (1);
	}
	return (0);
}

int	has_content_before_pipe(char *line, int pos)
{
	int	j;

	j = pos;
	while (j >= 0)
	{
		if (!ft_isspace(line[j]) && line[j] != '|')
			return (1);
		j--;
	}
	return (0);
}

int	is_double_pipe(char *line, int pipe_pos)
{
	int	j;

	j = pipe_pos - 1;
	while (j >= 0 && ft_isspace(line[j]))
		j--;
	if (j >= 0 && line[j] == '|')
		return (1);
	return (0);
}
