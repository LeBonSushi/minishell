/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jd-halle <jd-halle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 18:35:57 by macorso           #+#    #+#             */
/*   Updated: 2025/02/26 10:09:29 by jd-halle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*void	unset_value(t_env *env_to_unset)
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

void	ft_unset(t_token *tokens, t_env **env)
{
	if (!tokens || !tokens->next)
		return;
	t_token *current = tokens->next;
	t_env *start = *env;
	
	while (current)
	{
		start = *env;
		while (start)
		{
			if (!ft_strcmp(start->key, current->value))
			{
				unset_value(env, start);
				break ;
			}
			start = start->next;
		}
		current = current->next;
	}
}*/
