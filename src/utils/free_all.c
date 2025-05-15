/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jd-halle <jd-halle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 04:26:23 by jd-halle          #+#    #+#             */
/*   Updated: 2025/05/15 06:03:23 by jd-halle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_all_cmds(t_cmd *cmds)
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

void	free_all_env(t_env *env)
{
	t_env	*next;

	if (!env)
		return ;
	while (env)
	{
		next = env->next;
		free_env(env);
		env = next;
	}
}

void	free_all_token(t_token *token)
{
	t_token	*next;

	if (!token)
		return ;
	while (token)
	{
		next = token->next;
		if (token->value)
			free(token->value);
		free(token);
		token = next;
	}
}

void	free_all_cmd(t_cmd *cmd)
{
	t_cmd	*next;

	if (!cmd)
		return ;
	while (cmd)
	{
		next = cmd->next;
		free_all_token(cmd->tokens);
		if (cmd->args)
			free_tab(cmd->args);
		if (cmd->path)
			free(cmd->path);
		free(cmd);
		cmd = next;
	}
}

void	free_all(t_all *all)
{
	if (!all)
		return ;
	if (all->env)
		free_all_env(all->env);
	if (all->tokens)
		free_all_token(all->tokens);
	if (all->cmds)
		free_all_cmd(all->cmds);
	if (all->paths)
		free_tab(all->paths);
}