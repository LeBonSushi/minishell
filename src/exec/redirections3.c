/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:53:24 by jd-halle          #+#    #+#             */
/*   Updated: 2025/05/26 22:21:28 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	manage_redirections(t_all *all, t_cmd *cmds)
{
	t_cmd	*curr;
	int		status;

	if (!cmds)
		return (0);
	curr = cmds;
	status = 0;
	while (curr)
	{
		status = manage_cmd_redir(all, curr);
		curr = curr->next;
	}
	return (status);
}
