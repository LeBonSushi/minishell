/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtins.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 01:58:22 by jd-halle          #+#    #+#             */
/*   Updated: 2025/04/28 16:55:53 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void execute_builtins(char **args, t_env **env, t_all *all)
{
	if (!args)
		return;
	if (!ft_strcmp(args[0], "echo"))
		ft_echo(args);
	else if (!ft_strcmp(args[0], "cd"))
		ft_cd(args, *env);
	else if (!ft_strcmp(args[0], "pwd"))
		ft_pwd();
	else if (!ft_strcmp(args[0], "c"))
		clear_terminal();
	else if (!ft_strcmp(args[0], "export"))
		ft_export(args, env);
	else if (!ft_strcmp(args[0], "env"))
		ft_env(all->env);
	else if (!ft_strcmp(args[0], "exit"))
		ft_exit(args, all);
	/*else if (!ft_strcmp(args->value, "unset"))
		ft_unset(env);*/
		
}

bool	is_builtin(char *cmd)
{
	return (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "pwd")
		|| !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "env") || !ft_strcmp(cmd, "exit") || !ft_strcmp(cmd, "unset"));
}