/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:51:50 by jd-halle          #+#    #+#             */
/*   Updated: 2025/05/15 00:18:41 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	exec_cmds(t_all *all, t_cmd *cmds)
{
	t_cmd	*curr;
	int		fd[2];
	char	**paths;
	int		saved_fd[2];
	char	**env;

	if (!cmds)
		return ;
	curr = cmds;
	set_execution_mode();
	while (curr)
	{
		env = env_to_tab(all->env);
		paths = get_paths(all, env);
		curr->args = tokens_to_tab(curr->tokens);
		if (!curr->args)
			curr->path = NULL;
		else
			curr->path = find_exec_path(curr->args[0], paths);
		free_tab(paths);
		if (!curr->path || !curr->args[0] || curr->skip_cmd)
		{
			free_tab(env);
			// free_tab(curr->args);
			curr = curr->next;
			if (curr && curr->infile_fd < 0)
				curr->infile_fd = open("/dev/null", O_RDONLY);
			continue ;
		}
		update_env(&all->env, "_", curr->args[0], true);
		if (cmds && !cmds->next && is_builtin(curr->args[0]))
        {
            // Gérer les redirections pour les builtins
            saved_fd[0] = dup(STDIN_FILENO);
            saved_fd[1] = dup(STDOUT_FILENO);
            
            // Rediriger l'entrée si nécessaire
            if (curr->infile_fd > 0)
            {
                dup2(curr->infile_fd, STDIN_FILENO);
                close(curr->infile_fd);
            }
            
            // Rediriger la sortie si nécessaire
            if (curr->outfile_fd > 0)
            {
                dup2(curr->outfile_fd, STDOUT_FILENO);
                close(curr->outfile_fd);
            }
            
            // Exécuter le builtin
            execute_builtins(curr, curr->args, &all->env, all);
            
            // Restaurer les descripteurs standard
            dup2(saved_fd[0], STDIN_FILENO);
            dup2(saved_fd[1], STDOUT_FILENO);
            close(saved_fd[0]);
            close(saved_fd[1]);
        }
		else
			do_pipes(all, curr, env, fd);
		free_tab(env);
		curr = curr->next;
	}
	waitall(all, cmds);
	set_interactive_mode();
}
