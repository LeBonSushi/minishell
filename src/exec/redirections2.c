/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jd-halle <jd-halle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:53:14 by jd-halle          #+#    #+#             */
/*   Updated: 2025/04/29 22:06:12 by jd-halle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	do_heredoc(t_cmd *cmd, t_token *curr_token, bool is_last)
{
	char	*line;
	int		heredoc_fd;
	char	*limiter;
	char	*heredoc_file;
	int		i;
	char	*tmp;

	if (cmd->infile_fd > 0)
		close(cmd->infile_fd);
	i = 0;
	heredoc_file = ft_strdup(".heredoc");
	while (access(heredoc_file, F_OK) == 0)
	{
		tmp = heredoc_file;
		heredoc_file = ft_strjoin(".heredoc", ft_itoa(i++), 0, 1);
		free(tmp);
	}
	heredoc_fd = open(heredoc_file, O_RDWR | O_TRUNC | O_CREAT, 0664);
	limiter = ft_strdup(curr_token->next->value);
	while (1)
	{
		line = readline("ministress heredoc> ");
		if (!line || ft_compare(limiter, line))
		{
			if (line)
				free(line);
			free(limiter);
			break ;
		}
		ft_fprintf(heredoc_fd, "%s\n", line);
		free(line);
	}
	cmd->here_doc = true;
	close(heredoc_fd);
	if (is_last)
		cmd->infile_fd = open(heredoc_file, O_RDONLY);
	unlink(heredoc_file);
	free(heredoc_file);
	return (0);
}

bool	is_last(t_token *token)
{
	t_token	*curr;

	curr = token;
	while (curr->next)
	{
		if (curr->next->token_type == token->token_type)
			return (false);
		curr = curr->next;
	}
	return (true);
}

int	redir_in(t_cmd *curr_cmd, t_token *curr_token)
{
	t_token	*file;
	char	*error;

	if (curr_cmd->infile_fd > 0)
		close(curr_cmd->infile_fd);
	file = curr_token->next;
	error = ft_strjoin("ministress: ", file->value, 0, 0);
	if (access(file->value, F_OK | R_OK))
	{
		curr_cmd->skip_cmd = true;
		return (perror(error), free(error), 1);
	}
	curr_cmd->infile_fd = open(file->value, O_RDONLY);
	return (0);
}
