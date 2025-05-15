/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_prepare.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jd-halle <jd-halle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 14:26:45 by macorso           #+#    #+#             */
/*   Updated: 2025/05/15 10:44:02 by jd-halle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*cmd_new(t_token *tokens)
{
	t_cmd	*new;

	new = malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
	new->tokens = tokens;
	new->infile_fd = -2;
	new->outfile_fd = -2;
	new->path = NULL;
	new->args = NULL;
	new->here_doc = false;
	new->skip_cmd = false;
	new->pid = -2;
	new->next = NULL;
	return (new);
}

void	cmd_addback(t_cmd **cmd, t_cmd *new)
{
	t_cmd	*curr;

	if (!*cmd)
	{
		*cmd = new;
		return ;
	}
	curr = *cmd;
	while (curr->next)
		curr = curr->next;
	curr->next = new;
}
