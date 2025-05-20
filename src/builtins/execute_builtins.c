/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtins.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebonsushi <lebonsushi@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 01:58:22 by jd-halle          #+#    #+#             */
/*   Updated: 2025/05/20 14:46:55 by lebonsushi       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_builtins(t_cmd *cmd, char **args, t_env **env, t_all *all)
{
	if (!args)
		return ;
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
		ft_exit(cmd->tokens, all);
	else if (!ft_strcmp(args[0], "unset"))
		ft_unset(cmd->tokens, env);
}

bool	is_builtin(char *cmd)
{
	return (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "env") || !ft_strcmp(cmd, "exit")
		|| !ft_strcmp(cmd, "unset"));
}
