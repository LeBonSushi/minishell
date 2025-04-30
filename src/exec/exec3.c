/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jd-halle <jd-halle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:51:03 by jd-halle          #+#    #+#             */
/*   Updated: 2025/04/29 22:47:58 by jd-halle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_exit2(t_all *all, int exit_code)
{
	all->status = exit_code;
	exit(exit_code);
}

void	child_process(t_all *all, t_cmd *cmd, int *fd, char **env)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	close(fd[0]);
	if (cmd->infile_fd > 0)
		dup2(cmd->infile_fd, STDIN_FILENO);
	if (cmd->outfile_fd > 0)
		dup2(cmd->outfile_fd, STDOUT_FILENO);
	else if (cmd->next)
		dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	if (cmd->infile_fd > 0)
		close(cmd->infile_fd);
	if (cmd->outfile_fd > 0)
		close(cmd->outfile_fd);
	if (is_builtin(cmd->args[0]))
		execute_builtins(cmd, cmd->args, &all->env, all);
	else if (execve(cmd->path, cmd->args, env) == -1)
		exec_error(all, cmd->args[0]);
	free_all(all);
	free_tab(env);
	exit(all->status);
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

void	waitall(t_all *all, t_cmd *cmds)
{
	t_cmd	*cmd;
	int		tmp;

	cmd = cmds;
	tmp = 0;
	while (cmd)
	{
		waitpid(cmd->pid, &tmp, 0);
		cmd = cmd->next;
	}
	if (WIFEXITED(tmp))
		all->status = WEXITSTATUS(tmp);
	contain_redir_errors(cmds, &all->status, false);
}

void	do_pipes(t_all *all, t_cmd *curr, char **env, int fd[2])
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
