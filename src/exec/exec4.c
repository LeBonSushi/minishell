/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jd-halle <jd-halle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:51:50 by jd-halle          #+#    #+#             */
/*   Updated: 2025/04/29 22:48:54 by jd-halle         ###   ########.fr       */
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
			curr = curr->next;
			if (curr && curr->infile_fd < 0)
				curr->infile_fd = open("/dev/null", O_RDONLY);
			if (env)
				free_tab(env);
			if (paths)
				free_tab(paths);
			if (curr && curr->args)
				free_tab(curr->args);
			continue ;
		}
		all->paths = paths;
		if (is_builtin(curr->args[0]) && !cmds->next)
			execute_builtins(curr, curr->args, &all->env, all);
		else
			do_pipes(all, curr, env, fd);
		if (env)
			free_tab(env);
		if (paths)
			free_tab(paths);
		if (curr->args)
			free_tab(curr->args);
		curr = curr->next;
	}
	waitall(all, cmds);
}