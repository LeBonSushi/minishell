/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dfeve <dfeve@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 18:49:42 by macorso           #+#    #+#             */
/*   Updated: 2025/01/27 23:19:46 by dfeve            ###   ########.fr       */
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

void	free_command(t_command *com)
{
	free_tab(com->args);
	if (com->token)
		free(com->token);
}

void	free_all_env(t_all *all)
{
	t_env	*tmp;
	t_env	*next;

	tmp = all->env;
	while (tmp)
	{
		next = tmp->next;
		free_env(tmp);
		tmp = next;
	}
}