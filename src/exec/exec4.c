/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:51:50 by jd-halle          #+#    #+#             */
/*   Updated: 2025/05/06 12:28:56 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	exec_cmds(t_all *all, t_cmd *cmds)
{
	t_cmd	*curr;
	int		fd[2];
	char	**paths;
	char	**env;

	if (!cmds)
		return ;
	curr = cmds;
	set_execution_mode();
	while (curr)
	{
		env = env_to_tab(all->env);
		paths = get_paths(all, env);
		curr->args = tokens_to_tab(curr->tokens);
		if (!curr->args)
			curr->path = NULL;
		else
			curr->path = find_exec_path(curr->args[0], paths);
		if (!curr->path || !curr->args[0] || curr->skip_cmd)
		{
			free_tab(env);
			free_tab(paths);
			free_tab(curr->args);
			curr = curr->next;
			if (curr && curr->infile_fd < 0)
				curr->infile_fd = open("/dev/null", O_RDONLY);
			continue ;
		}
		all->paths = paths;
		
		do_pipes(all, curr, env, fd);
		update_env(&all->env, "_", curr->args[0], true);
		free_tab(env);
		free_tab(paths);
		free_tab(curr->args);
		curr = curr->next;
	}
	waitall(all, cmds);
	set_interactive_mode();
}
