/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   continuation_readline.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 22:40:39 by macorso           #+#    #+#             */
/*   Updated: 2025/05/27 01:22:36 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*read_one_continuation(char *prompt,
	char *full_line, t_all *all, int fd)
{
	char	*continuation_line;

	continuation_line = readline(prompt);
	if (!continuation_line && g_exit == 99)
	{
		dup2(fd, 0);
		g_exit = 130;
		signal(SIGINT, &signal_heredoc);
		free(full_line);
		return (NULL);
	}
	if (!continuation_line)
	{
		print_error_continuation(full_line, all);
		free(full_line);
		return (NULL);
	}
	return (join_lines_nl(full_line, continuation_line));
}

char	*handle_line_continuation(char *line, t_all *all)
{
	char	*full_line;
	char	*prompt;
	int		fd;

	if (!line)
		return (NULL);
	full_line = ft_strdup(line);
	if (!full_line)
		return (NULL);
	signal(SIGINT, signal_heredoc);
	fd = dup(0);
	while (1)
	{
		prompt = get_continuation_prompt(full_line);
		if (!prompt)
			break ;
		full_line = read_one_continuation(prompt, full_line, all, fd);
		if (!full_line)
			return (NULL);
	}
	return (full_line);
}
