/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebonsushi <lebonsushi@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:51:50 by jd-halle          #+#    #+#             */
/*   Updated: 2025/05/20 14:44:49 by lebonsushi       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	tab_len(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

void	exec_solo_buitin(char **env, t_cmd *curr, t_all *all)
{
	int		saved_fd[2];

	saved_fd[0] = dup(STDIN_FILENO);
	saved_fd[1] = dup(STDOUT_FILENO);
	if (curr->infile_fd > 0)
	{
		dup2(curr->infile_fd, STDIN_FILENO);
		close(curr->infile_fd);
	}
	if (curr->outfile_fd > 0)
	{
		dup2(curr->outfile_fd, STDOUT_FILENO);
		close(curr->outfile_fd);
	}
	free_tab(env);
	execute_builtins(curr, curr->args, &all->env, all);
	dup2(saved_fd[0], STDIN_FILENO);
	dup2(saved_fd[1], STDOUT_FILENO);
	close(saved_fd[0]);
	close(saved_fd[1]);
}

int	exec_cmd(t_all *all, t_cmd *curr, t_cmd *cmds, int fd[2])
{
	char	**env;
	char	**paths;

	env = env_to_tab(all->env);
	paths = get_paths(all, env);
	curr->args = tokens_to_tab(curr->tokens);
	curr->path = find_exec_path(curr->args[0], paths, curr->args);
	free_tab(paths);
	if (!curr->path || !curr->args[0] || curr->skip_cmd)
	{
		free_tab(env);
		curr = curr->next;
		if (curr && curr->infile_fd < 0)
			curr->infile_fd = open("/dev/null", O_RDONLY);
		return (-1);
	}
	update_env(&all->env, "_", curr->args[tab_len(curr->args) - 1], true);
	if (cmds && !cmds->next && is_builtin(curr->args[0]))
		exec_solo_buitin(env, curr, all);
	else
	{
		do_pipes(all, curr, env, fd);
		free_tab(env);
	}
	return (0);
}

void	exec_cmds(t_all *all, t_cmd *cmds)
{
	t_cmd	*curr;
	int		fd[2];

	if (!cmds)
		return ;
	curr = cmds;
	set_execution_mode();
	while (curr)
	{
		if (exec_cmd(all, curr, cmds, fd) == -1)
			continue ;
		curr = curr->next;
	}
	waitall(cmds);
	set_interactive_mode();
}
