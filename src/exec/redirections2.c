/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:53:18 by jd-halle          #+#    #+#             */
/*   Updated: 2025/05/27 13:50:06 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	if (curr_cmd->outfile_fd == -1)
	{
		curr_cmd->skip_cmd = true;
		return (ft_putstr_fd("minishell: ", 2), perror(file->value), 1);
	}
	return (0);
}

int	get_redir_status(t_all *all, t_cmd *curr_cmd, t_token *curr_token, int s)
{
	int	status;

	status = s;
	if (curr_token->token_type == REDIR_HEREDOC)
		status = do_heredoc(all, curr_cmd, curr_token);
	else if (curr_token->token_type == REDIR_IN && !curr_cmd->skip_cmd)
		status = redir_in(curr_cmd, curr_token);
	else if ((curr_token->token_type == REDIR_APPEND
			|| curr_token->token_type == REDIR_OUT)
		&& !curr_cmd->skip_cmd)
		status = redir_out_append(curr_cmd, curr_token);
	return (status);
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
			status = get_redir_status(all, curr_cmd, curr_token, status);
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
	return (token->token_type == REDIR_APPEND
		|| token->token_type == REDIR_OUT);
}
