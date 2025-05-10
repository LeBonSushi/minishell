/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:51:03 by jd-halle          #+#    #+#             */
/*   Updated: 2025/05/10 02:26:52 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_exit2(t_all *all, int exit_code)
{
	(void)all;
	g_zebi.g_exit = exit_code;
	exit(exit_code);
}

void child_process(t_all *all, t_cmd *cmd, int *fd, char **env)
{
	ft_fprintf(2, "ca rentre childproc\n");
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	close(fd[0]);

	printf("%p\n", all);
	printf("%p\n", cmd);
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
		ft_fprintf(2, "ca rentre isbuiltin childproc\n");
		print_tab(cmd->args);
		execute_builtins(cmd, cmd->args, &all->env, all);
	}
	else
	{
		// print_tab(cmd->args);
		execve(cmd->path, cmd->args, env);
		exec_error(all, cmd->args[0]);
	}
	free_tab(env);
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

// void	waitall(t_all *all, t_cmd *cmds)
// {
// 	(void)all;
// 	t_cmd	*cmd;
// 	int		tmp;

// 	cmd = cmds;
// 	tmp = -1;
// 	while (cmd)
// 	{
// 		if (cmd->skip_cmd == true)
// 		{
// 			cmd = cmd->next;
// 			continue ;
// 		}
// 		if (cmd->pid > 0)
// 			waitpid(cmd->pid, &tmp, 0);
// 		cmd = cmd->next;
// 	}
// 	if (WIFEXITED(tmp))
// 		g_zebi.g_exit = WEXITSTATUS(tmp);
// 	contain_redir_errors(cmds, &g_zebi.g_exit, false);
// }

void waitall(t_all *all, t_cmd *cmds)
{
	t_cmd *curr = cmds;
	int status;
	bool signal_interrupt = false;
	int i = 0;
	(void)all;
	
	while (curr)
	{
		while (curr->args[i])
		{
			ft_fprintf(2, "[%d]%s ", i,curr->args[i]);
			i++;
		}
		curr = curr->next;
	}
	curr = cmds;
	while (curr)
	{
		printf("\n");
		
		if (curr->pid > 0)
		{
			waitpid(curr->pid, &status, 0);
			if (WIFEXITED(status))
				g_zebi.g_exit = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				g_zebi.g_exit = 128 + WTERMSIG(status);
				if (WTERMSIG(status) == SIGINT)
					signal_interrupt = true;
				else if (WTERMSIG(status) == SIGQUIT && !curr->next)
					write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
			}
		}
		curr = curr->next;
	}
	if (signal_interrupt && g_zebi.g_exit != 130)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
	}
	contain_redir_errors(cmds, &g_zebi.g_exit, false);
}

void	do_pipes(t_all *all, t_cmd *curr, char **env, int fd[2])
{
	print_tab(curr->args);
	if (is_builtin(curr->args[0]) && !all->cmds->next)
	{
			printf("rentre is builtin dopipes\n");
			execute_builtins(curr, curr->args, &all->env, all);
	}
	else
	{
		if (pipe(fd) == -1)
			pipe_error(all, fd);
		curr->pid = fork();
		if (curr->pid == -1)
			fork_error(all, fd);
		if (curr->pid == 0)
		{
			child_process(all, curr, fd, env);
		}
		else
			parent_process(curr, fd);
	}
}
