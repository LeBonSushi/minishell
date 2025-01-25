/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 18:35:57 by macorso           #+#    #+#             */
/*   Updated: 2025/01/25 18:52:26 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	unset_builtin(char *key, t_all *all)
{
	t_env	*env;
	t_env	*tmp;

	env = all->env;
	if (!env)
		return ;
	if (!ft_strcmp(env->key, key))
	{
		tmp = env;
		all->env = env->next;
		free_env(tmp);
		return ;
	}
	while (env->next)
	{
		if (!ft_strcmp(env->next->key, key))
		{
			tmp = env->next;
			env->next = env->next->next;
			free_env(tmp);
			return ;
		}
		env = env->next;
	}
}
