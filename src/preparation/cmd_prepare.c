/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_prepare.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jd-halle <jd-halle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 14:26:45 by macorso           #+#    #+#             */
/*   Updated: 2025/03/01 21:56:06 by jd-halle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_cmd	*cmd_new(t_token	*tokens, char *path)
{
	t_cmd	*new;

	new = malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
	new->tokens = tokens;
	new->infile_fd = 0;
	new->outfile_fd = 1;
	new->path = ft_strdup(path);
	new->skip_cmd = false;
	new->pid = -2;
	new->next = NULL;
	return (new);
}

void	cmd_addback(t_cmd **cmd, t_cmd *new)
{
	t_cmd *curr;

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