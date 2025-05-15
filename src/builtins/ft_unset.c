/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jd-halle <jd-halle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 18:35:57 by macorso           #+#    #+#             */
/*   Updated: 2025/05/15 10:59:32 by jd-halle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	unset_value(t_env **env_head, t_env *env_to_unset)
{
	t_env	*env;
	t_env	*tmp;

	if (!env_head || !*env_head || !env_to_unset)
		return ;
	env = *env_head;
	if (env == env_to_unset)
	{
		*env_head = env->next;
		free_env(env);
		return ;
	}
	while (env->next && env->next != env_to_unset)
		env = env->next;
	if (env->next)
	{
		tmp = env->next;
		env->next = tmp->next;
		free_env(tmp);
	}
}

void	ft_unset(t_token *tokens, t_env **env)
{
	t_token	*current;
	t_env	*start;

	if (!tokens || !tokens->next || !env || !*env)
		return ;
	current = tokens->next;
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
}
