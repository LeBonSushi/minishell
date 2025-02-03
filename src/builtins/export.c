/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 15:43:10 by macorso           #+#    #+#             */
/*   Updated: 2025/02/02 17:59:59 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	verify_addkey(t_all *all, char *key, char *value)
{
	t_env	*env;

	printf("key: %s\n", key);
	printf("value: %s\n", value);
	env = all->env;
	while (env)
	{
		if (!ft_strcmp(env->key, key))
		{
			free(env->value);
			env->value = ft_strdup(value);
			return ;
		}
		env = env->next;
	}
	env_addback(&all->env, env_new(key, value));
}

void	env_swap(t_env *env1, t_env *env2)
{
	char	*tmp_key;
	char	*tmp_value;

	tmp_key = env1->key;
	tmp_value = env1->value;
	env1->key = env2->key;
	env1->value = env2->value;
	env2->key = tmp_key;
	env2->value = tmp_value;
}

t_env	*env_copy(t_env *env)
{
	t_env	*new;
	t_env	*tmp;

	new = NULL;
	while (env)
	{
		tmp = env_new(env->key, env->value);
		env_addback(&new, tmp);
		env = env->next;
	}
	return (new);
}

t_env *env_create_sorted(t_env *env)
{
	t_env *curr;
	t_env *new;
	bool	swapped;

	new = env_copy(env);
	swapped = true;
	while (swapped)
	{
		swapped = false;
		curr = new;
		while (curr->next)
		{
			if (ft_strcmp(curr->key, curr->next->key) > 0)
			{
				env_swap(curr, curr->next);
				swapped = true;
			}
			curr = curr->next;
		}
	}
	return (new);
}

void	export_builtin(t_all *all, char **strs)
{
	int		i;
	char	*key;
	char	*value;
	t_env	*env;

	if (!all->env || !strs)
		return ;
	i = -1;
	if (!*strs)
	{
		env = env_create_sorted(all->env);
		while (env)
		{
			if (env->value)
				printf("declare -x %s=\"%s\"\n", env->key, env->value);
			else
				printf("declare -x %s\n", env->key);
			env = env->next;
		}
		free_all_env(env);
		return ;
	}
	while (strs[++i])
	{
		key = find_key(strs[i]);
		if (!key)
			continue;
		value = find_value(strs[i]);
		env = all->env;
		verify_addkey(all, key, value);
		free(key);
		free(value);
	}
}
