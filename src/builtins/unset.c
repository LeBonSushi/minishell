/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 18:35:57 by macorso           #+#    #+#             */
/*   Updated: 2025/01/26 17:16:38 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	unset_value(t_all *all, t_env *env_to_unset)
{
	t_env	*env;
	t_env	*tmp;
	
	env = all->env;
	if (env == env_to_unset)
	{
		all->env = env->next;
		free_env(env);
		return ;
	}
	while (env->next != env_to_unset)
		env = env->next;
	tmp = env->next;
	env->next = tmp->next;
	free_env(tmp);
}

void	unset_builtin(char **keys, t_all *all)
{
	t_env	*env;
	int		i;
	
	i = 0;
	while (keys[i])
	{
		env = all->env;
		while (env)
		{
			if (!ft_strcmp(env->key, keys[i]))
			{
				unset_value(all, env);
				break ;
			}
			env = env->next;
		}
		i++;
	}
}
