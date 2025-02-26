/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jd-halle <jd-halle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 18:49:42 by macorso           #+#    #+#             */
/*   Updated: 2025/02/25 22:25:42 by jd-halle         ###   ########.fr       */
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

void	free_all(t_all *all)
{
	free_all_env(all->env);
	free_all_token(all->tokens);
}