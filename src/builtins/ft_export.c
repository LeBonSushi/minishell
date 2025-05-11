/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jd-halle <jd-halle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 15:43:10 by macorso           #+#    #+#             */
/*   Updated: 2025/04/29 22:14:56 by jd-halle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	find_egal_in_token(char *env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (env[i] == '=')
			return (true);
		i++;
	}
	return (false);
}

t_env	*env_find(t_env *env, char *key)
{
	t_env	*curr;

	curr = env;
	while (curr)
	{
		if (ft_strcmp(curr->key, key) == 0)
			return (curr);
		curr = curr->next;
	}
	return (NULL);
}

void	ft_export(t_token *tokens, t_env **env)
{
	char	*key;
	char	*value;
	t_env	*existing;
	t_token	*current;
	t_env	*env_sorted;

	env_sorted = *env;
	if (!tokens || !env || !(*env))
		return ;
	current = tokens->next;
	if (!current)
	{
		env_sorted = env_create_sorted(env_sorted);
		return (print_env (env_sorted));
	}
	while (current)
	{
		if (find_egal_in_token(current->value))
		{
			key = find_key(current->value);
			value = find_value(current->value);
			if (!check_valid_key_export(key))
			{
				free(key);
				free(value);
				current = current->next;
				continue ;
			}
			existing = env_find(*env, key);
			if (existing)
			{
				free(existing->value);
				existing->value = value;
			}
			else
				env_addback(env, env_new(key, value));
		}
		else
		{
			key = find_key(current->value);
			if (!check_valid_key_export(current->value))
			{
				free(key);
				current = current->next;
				continue ;
			}
			if (!env_find(*env, key))
				env_addback(env, env_new(key, NULL));
			else
				free(key);
		}
		current = current->next;
	}
	env_sorted = env_create_sorted(env_sorted);
}
