/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:51:03 by jd-halle          #+#    #+#             */
/*   Updated: 2025/05/24 20:25:21 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	free_multiple(all, env);
	exit(g_zebi.g_exit);
}

void	parent_process(t_cmd *curr, int *fd)
{
	close(fd[1]);
	if (curr->infile_fd > 0)
		close(curr->infile_fd);
	if (curr->next && curr->next->infile_fd < 0)
		curr->next->infile_fd = dup(fd[0]);
	close(fd[0]);
}

int	wait_all_exitcode(t_cmd *cmd, int tmp)
{
	int	exit;

	exit = g_zebi.g_exit;
	if (WIFEXITED(tmp))
		exit = WEXITSTATUS(tmp);
	else if (WIFSIGNALED(tmp))
	{
		exit = WTERMSIG(tmp) + 128;
		if (WTERMSIG(tmp) == SIGINT)
			write(1, "\n", 1);
		else if (WTERMSIG(tmp) == SIGQUIT)
			write(1, "Quit (core dumped)\n", 19);
		else if (WTERMSIG(tmp) == SIGSEGV)
			ft_fprintf(2, "[1]    %d segmentation fault (core dumped)  %s\n",
				cmd->pid, cmd->args[0]);
		else if (WTERMSIG(tmp) == SIGPIPE)
		{
			if (!cmd->next)
				write(1, "Broken pipe\n", 13);
		}
	}
	return (exit);
}

void	waitall(t_cmd *cmds)
{
	t_cmd	*cmd;
	int		tmp;

	cmd = cmds;
	tmp = -1;
	while (cmd)
	{
		if (cmd->skip_cmd == true)
		{
			cmd = cmd->next;
			continue ;
		}
		waitpid(cmd->pid, &tmp, 0);
		g_zebi.g_exit = wait_all_exitcode(cmd, tmp);
		cmd = cmd->next;
	}
	contain_redir_errors(cmds, &g_zebi.g_exit, false);
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
