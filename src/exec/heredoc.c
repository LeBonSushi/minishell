/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 13:51:59 by macorso           #+#    #+#             */
/*   Updated: 2025/05/22 13:52:30 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	signal_heredoc(int sig)
{
	if (sig == SIGINT)
	{
		rl_on_new_line();
		close(0);
		g_zebi.g_exit = 99;
	}
}

int	here_doc_eof(t_all *all, char *limiter)
{
	ft_fprintf(2, "error: warning: here-document at line %d "
		"delimited by end-of-line (wanted `%s')\n", all->num_line, limiter);
	g_zebi.g_exit = 0;
	return (1);
}

void free_write(int *pipe_fd, char *input)
{
	write(pipe_fd[1], input, ft_strlen(input));
	write(pipe_fd[1], "\n", 1);
	free(input);
}

int	end_heredoc(char *input, int *pipe_fd, t_cmd *cmd)
{
	free(input);
	close(pipe_fd[1]);
	if (cmd->infile_fd > 0)
		close(cmd->infile_fd);
	cmd->infile_fd = pipe_fd[0];
	return (0);
}

int	do_heredoc(t_all *all, t_cmd *cmd, t_token *curr_token)
{
	int		pipe_fd[2];
	char	*input;
	int		fd;

	fd = dup(0);
	signal(SIGINT, signal_heredoc);
	if (pipe(pipe_fd) == -1)
		return (pipe_error(all, pipe_fd), 1);
	while (1)
	{
		input = readline("> ");
		if (!input && g_zebi.g_exit == 99)
		{
			dup2(fd, 0);
			g_zebi.g_exit = 130;
			signal(SIGINT, &signal_heredoc);
			break ;
		}
		if (!input && here_doc_eof(all, curr_token->next->value))
			break ;
		if (ft_compare(curr_token->next->value, input))
			break ;
		free_write(pipe_fd, input);
	}
	return (end_heredoc(input, pipe_fd, cmd));
}