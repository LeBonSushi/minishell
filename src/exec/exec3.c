/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:51:03 by jd-halle          #+#    #+#             */
/*   Updated: 2025/05/15 01:52:37 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_exit2(t_all *all, int exit_code)
{
	(void)all;
	g_zebi.g_exit = exit_code;
	exit(exit_code);
}

void	free_all_cmds2(t_cmd *cmds)
{
	t_cmd	*curr;
	t_cmd	*next;

	if (!cmds)
		return ;
	curr = cmds;
	while (curr)
	{
		next = curr->next;
		if (curr->path)
			free(curr->path);
		if (curr->tokens)
			free_all_token(curr->tokens);
		if (curr->args)
			free_tab(curr->args);
		free(curr);
		curr = next;
	}
	
}

void child_process(t_all *all, t_cmd *cmd, int *fd, char **env)
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
        execute_builtins(cmd, cmd->args, &all->env, all);
    else
    {
        execve(cmd->path, cmd->args, env);
        exec_error(all, cmd->args[0]);
    }
	free_all_cmds2(all->cmds);
	free_all_token(all->tokens);
	free_all_env(all->env);
    free_tab(env);
	rl_clear_history();
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

void	waitall(t_all *all, t_cmd *cmds)
{
	t_cmd	*cmd;
	(void) all;
	int		tmp;

	cmd = cmds;
	// if (cmds && !cmds->next && is_builtin(cmds->args[0]))
	// 	return ;
	tmp = -1;
	// printf("debut: %d\n", g_zebi.g_exit);
	while (cmd)
	{
		// printf("skip: %d\n", cmd->skip_cmd);
		if (cmd->skip_cmd == true)
		{
			cmd = cmd->next;
			continue ;
		}
		waitpid(cmd->pid, &tmp, 0);
		// printf("%d\n", tmp);
		cmd = cmd->next;
	}
	// printf("ouais: %d\n", tmp);
	if (WIFEXITED(tmp))
		g_zebi.g_exit = WEXITSTATUS(tmp);
	else if (WIFSIGNALED(tmp))
	{
		g_zebi.g_exit = WTERMSIG(tmp) + 128;
		if (WTERMSIG(tmp) == SIGINT)
			write(2, "\n", 1);
		else if (WTERMSIG(tmp) == SIGQUIT)
			write(2, "Quit (core dumped)\n", 19);
	}
	contain_redir_errors(cmds, &g_zebi.g_exit, false);
}

void	do_pipes(t_all *all, t_cmd *curr, char **env, int fd[2])
{
	// if (is_builtin(curr->args[0]) && !curr->next)
	// 		execute_builtins(curr, curr->args, &all->env, all);
	// else
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
