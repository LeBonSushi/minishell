/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:53:14 by macorso           #+#    #+#             */
/*   Updated: 2025/05/27 01:22:36 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

	exit = g_exit;
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
		g_exit = wait_all_exitcode(cmd, tmp);
		cmd = cmd->next;
	}
}
