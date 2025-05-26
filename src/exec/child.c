/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:55:39 by macorso           #+#    #+#             */
/*   Updated: 2025/05/27 01:22:36 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_child_proc(t_all *all, t_cmd *cmd, char **env)
{
	if (is_builtin(cmd->args[0]))
	{
		if (!ft_strcmp(cmd->args[0], "exit"))
			free_tab(env);
		pipe_execute_builtins(cmd, cmd->args, &all->env, all);
	}
	else
	{
		execve(cmd->path, cmd->args, env);
		exec_error(cmd->args[0]);
	}
}

void	child_process(t_all *all, t_cmd *cmd, int *fd, char **env)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	close(fd[0]);
	if (cmd->infile_fd > 0)
	{
		dup2(cmd->infile_fd, STDIN_FILENO);
		close(cmd->infile_fd);
	}
	if (cmd->outfile_fd > 0)
	{
		dup2(cmd->outfile_fd, STDOUT_FILENO);
		close(cmd->outfile_fd);
	}
	else if (cmd->next)
		dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	exec_child_proc(all, cmd, env);
	free_multiple(all, env);
	exit(g_exit);
}

void	do_pipes(t_all *all, t_cmd *curr, char **env, int fd[2])
{
	{
		if (pipe(fd) == -1)
			pipe_error(all, fd);
		curr->pid = fork();
		if (curr->pid == -1)
			fork_error(all, fd);
		if (curr->pid == 0)
			child_process(all, curr, fd, env);
		else
			parent_process(curr, fd);
	}
}
