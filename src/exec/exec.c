/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:51:50 by jd-halle          #+#    #+#             */
/*   Updated: 2025/05/26 22:49:04 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	dup_fds(int *saved_fd)
{
	dup2(saved_fd[0], STDIN_FILENO);
	dup2(saved_fd[1], STDOUT_FILENO);
	close(saved_fd[0]);
	close(saved_fd[1]);
}

void	exec_cmd(t_cmd *curr, int *fd, t_all *all)
{
	char	**env;
	int		saved_fd[2];

	env = env_to_tab(all->env);
	if (all->cmds && !all->cmds->next && is_builtin(curr->args[0]))
	{
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
		execute_builtins(curr, env, &all->env, all);
		dup_fds(saved_fd);
	}
	else
		do_pipes(all, curr, env, fd);
	free_tab(env);
}

static void	prepare_cmd(t_all *all, t_cmd *cmd)
{
	char	**paths;
	char	**env;

	env = env_to_tab(all->env);
	paths = get_paths(all, env);
	free_tab(env);
	cmd->args = tokens_to_tab(cmd->tokens);
	if (!cmd->args)
		cmd->path = NULL;
	else
		cmd->path = find_exec_path(cmd->args[0], paths);
	free_tab(paths);
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
		prepare_cmd(all, curr);
		if (!curr->path || !curr->args[0] || curr->skip_cmd)
		{
			if (curr->next && curr->next->infile_fd < 0)
				curr->next->infile_fd = open("/dev/null", O_RDONLY);
			curr = curr->next;
			continue ;
		}
		update_env(&all->env, "_", curr->args[tab_len(curr->args) - 1], true);
		exec_cmd(curr, fd, all);
		curr = curr->next;
	}
	waitall(cmds);
	set_interactive_mode();
}
