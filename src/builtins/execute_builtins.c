/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtins.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jd-halle <jd-halle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 01:58:22 by jd-halle          #+#    #+#             */
/*   Updated: 2025/05/09 18:13:14 by jd-halle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void execute_builtins(t_cmd *cmd, char **args, t_env **env, t_all *all)
{
	if (!args)
		return;
	if (!ft_strcmp(args[0], "echo"))
		ft_echo(cmd->tokens);
	else if (!ft_strcmp(args[0], "cd"))
		ft_cd(cmd->tokens, *env);
	else if (!ft_strcmp(args[0], "pwd"))
		ft_pwd();
	else if (!ft_strcmp(args[0], "export"))
		ft_export(cmd->tokens, env);
	else if (!ft_strcmp(args[0], "env"))
		ft_env(all->env);
	else if (!ft_strcmp(args[0], "exit"))
	{
		ft_exit(cmd->tokens, all);
	}
	// close (cmd->infile_fd);
	// close (cmd->outfile_fd);
	/*else if (!ft_strcmp(args->value, "unset"))
		ft_unset(env);*/
}

bool	is_builtin(char *cmd)
{
	return (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "pwd")
		|| !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "env") || !ft_strcmp(cmd, "exit") || !ft_strcmp(cmd, "unset"));
}