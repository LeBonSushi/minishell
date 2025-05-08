/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jd-halle <jd-halle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:53:18 by jd-halle          #+#    #+#             */
/*   Updated: 2025/05/08 20:10:50 by jd-halle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	redir_append(t_cmd *curr_cmd, t_token *curr_token)
{
	t_token	*file;
	char	*error;

	if (curr_cmd->outfile_fd > 0)
		close(curr_cmd->outfile_fd);
	file = curr_token->next;
	error = ft_strjoin("ministress: ", file->value, 0, 0);
	curr_cmd->outfile_fd = open(file->value, O_RDWR | O_APPEND | O_CREAT, 0644);
	if (access(file->value, R_OK))
		return (perror(error), free(error), 1);
	return (0);
}

int	redir_out_append(t_cmd *curr_cmd, t_token *curr_token)
{
	t_token	*file;
	int		flags;
	mode_t	mode;

	if (curr_cmd->outfile_fd > 0)
		close(curr_cmd->outfile_fd);
	file = curr_token->next;
	if (!file || file->token_type != WORD)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token\n", 2);
		return (1);
	}
	mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	flags = O_WRONLY | O_CREAT;
	if (curr_token->token_type == REDIR_OUT)
		flags |= O_TRUNC;
	else if (curr_token->token_type == REDIR_APPEND)
		flags |= O_APPEND;
	curr_cmd->outfile_fd = open(file->value, flags, mode);
	// printf("out: %d\n", curr_cmd->outfile_fd);
	if (curr_cmd->outfile_fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(file->value);
		curr_cmd->skip_cmd = true;
		return (1);
	}
	return (0);
}

int	manage_cmd_redir(t_all *all, t_cmd *curr_cmd)
{
	t_token	*curr_token;
	t_token	*next;
	int		status;

	if (!curr_cmd)
		return (0);
	curr_token = curr_cmd->tokens;
	status = 0;
	while (curr_token && curr_token->next)
	{
		next = curr_token->next->next;
		if (is_redir(curr_token))
		{
			if (curr_token->token_type == REDIR_HEREDOC)
			{
				status = do_heredoc(curr_cmd, curr_token, is_last(curr_token));
				if (status == 130)
					return (status);  // Retourner immédiatement
			}
			else if (curr_token->token_type == REDIR_IN && !curr_cmd->skip_cmd)
				status = redir_in(all, curr_cmd, curr_token);
			else if ((curr_token->token_type == REDIR_APPEND
					|| curr_token->token_type == REDIR_OUT)
				&& !curr_cmd->skip_cmd)
				status = redir_out_append(curr_cmd, curr_token);
			delete_token(&curr_cmd->tokens, curr_token->next);
			delete_token(&curr_cmd->tokens, curr_token);
			curr_token = next;
		}
		else
			curr_token = curr_token->next;
	}
	return (status);
}

bool	is_redir_out(t_token *token)
{
	return (token->token_type == REDIR_APPEND || token->token_type == REDIR_OUT);
}
