/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 18:49:42 by macorso           #+#    #+#             */
/*   Updated: 2025/02/17 08:51:47 by macorso          ###   ########.fr       */
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

	i = 0;
	if (!tab && !*tab)
		return ;
	while (tab[i])
		free(tab[i++]);
	free(tab);
}

void	free_all_env(t_env *env)
{
	t_env	*next;

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
		free(token);
		token = next;
	}
}

void	free_tree(t_tree *tree)
{
	if (!tree)
		return ;
	free_tree(tree->left);
	free_tree(tree->right);
	ft_strclear(&tree->args);
	ft_strclear(&tree->redir_in_names);
	ft_strclear(&tree->redir_out_names);
	free(tree);
}

void	free_all(t_all *all)
{
	free_all_env(all->env);
	free_all_token(all->tokens);
}