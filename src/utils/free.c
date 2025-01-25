/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 18:49:42 by macorso           #+#    #+#             */
/*   Updated: 2025/01/25 18:52:43 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_env(t_env *env)
{
	free(env->key);
	free(env->value);
	free(env);
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