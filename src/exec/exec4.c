/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:51:50 by jd-halle          #+#    #+#             */
/*   Updated: 2025/05/10 06:15:23 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void handle_skip_case(t_cmd **curr, char **env, char **paths)
{
    if ((*curr)->next && (*curr)->next->infile_fd < 0)
        (*curr)->next->infile_fd = open("/dev/null", O_RDONLY);
    
    free_tab(env);
    free_tab(paths);
    free_tab((*curr)->args);
    *curr = (*curr)->next;
}

void execute_builtin_directly(t_all *all, t_cmd *cmd)
{
    int saved_stdin = dup(STDIN_FILENO);
    int saved_stdout = dup(STDOUT_FILENO);

    // Apply redirections
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

    execute_builtins(cmd, cmd->args, &all->env, all);

    // Restore standard FDs
    dup2(saved_stdin, STDIN_FILENO);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdin);
    close(saved_stdout);
}

void free_resources(char **env, char **paths, char **args)
{
    free_tab(env);
    free_tab(paths);
    if (args) free_tab(args);
}

void	exec_cmds(t_all *all, t_cmd *cmds)
{
	t_cmd	*curr;
	int		fd[2];
	char	**paths;
	char	**env;

	if (!cmds)
		return ;
	curr = cmds;
	// set_execution_mode();
	while (curr)
	{
		paths = get_paths(all, env_to_tab(all->env));
		curr->args = tokens_to_tab(curr->tokens);
		if (!curr->args)
			curr->path = NULL;
		else
			curr->path = find_exec_path(curr->args[0], paths);
		if(paths)
			free_tab(paths);
		if (!curr->path || !curr->args[0] || curr->skip_cmd)
		{
			curr = curr->next;
			if (curr && curr->infile_fd < 0)
				curr->infile_fd = open("/dev/null", O_RDONLY);
			free_tab(curr->args);
			continue ;
		}
		set_execution_mode();
		env = env_to_tab(all->env);
		do_pipes(all, curr, env, fd);
		update_env(&all->env, "_", curr->args[0], true);
		free_tab(curr->args);
		free_tab(env);
		curr = curr->next;
	}
	waitall(all, cmds);
	set_interactive_mode();
}
