/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jd-halle <jd-halle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:53:24 by jd-halle          #+#    #+#             */
/*   Updated: 2025/05/15 09:16:08 by jd-halle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	contain_redir_errors(t_cmd *cmds, int *status, bool print)
{
	t_token	*token;

	if (!cmds)
		return (false);
	while (cmds)
	{
		token = cmds->tokens;
		while (token)
		{
			if (is_redir_out(token) && !token->next)
			{
				*status = 2;
				if (cmds->next && print)
					ft_fprintf(2, "ministress: syntax error near unexpected token `|\'\n");
				else if (!cmds->next && print)
					ft_fprintf(2, "ministress: syntax error near unexpected token `newline\'\n");
				return (true);
			}
			else if (is_redir_out(token) && token->next->token_type != WORD)
			{
				*status = 2;
				if (print)
					ft_fprintf(2, "ministress: syntax error near unexpected token `%s\'\n", token->next->value);
				return (true);
			}
			token = token->next;
		}
		cmds = cmds->next;
	}
	return (false);
}

void	skip_all_cmds(t_cmd *cmds)
{
	if (!cmds)
		return ;
	while (cmds)
	{
		cmds->skip_cmd = true;
		cmds = cmds->next;
	}
}

int	manage_redirections(t_cmd *cmds)
{
	t_cmd	*curr;
	int		status;

	if (!cmds)
		return (0);
	curr = cmds;
	status = 0;
	if (contain_redir_errors(cmds, &status, true))
		return (skip_all_cmds(cmds), status);
	while (curr)
	{
		status = manage_cmd_redir(curr);
		curr = curr->next;
	}
	return (status);
}
