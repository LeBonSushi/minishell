/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 18:49:42 by macorso           #+#    #+#             */
/*   Updated: 2025/05/11 02:03:00 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_env(t_env *env)
{
	free(env->key);
	free(env->value);
	free(env);
}

void	free_tab(char **tab)
{
	int	i;

	if (!tab)
		return;
	i = 0;
	while (tab[i])
		free(tab[i++]);
	free(tab);
	tab = NULL;
}

void	free_all_env(t_env *env)
{
	t_env	*next;
	
	if (!env)
		return;
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
	t_cmd *next;
	
	if (!cmd)
		return;
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
		return;
	if (all->env)
		free_all_env(all->env);
	if (all->tokens)
		free_all_token(all->tokens);
	if (all->cmds)
		free_all_cmd(all->cmds);
	if (all->paths)
		free_tab(all->paths);
}

void	free_all2(t_all *all)
{
	if (!all)
		return;
	if (all->env)
		free_all_env(all->env);
	if (all->cmds)
		free_all_cmd(all->cmds);
}