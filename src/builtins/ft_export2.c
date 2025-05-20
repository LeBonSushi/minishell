/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebonsushi <lebonsushi@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 22:10:59 by jd-halle          #+#    #+#             */
/*   Updated: 2025/05/20 11:07:07 by lebonsushi       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_env(t_env *env)
{
	t_env	*curr;

	curr = env;
	while (curr)
	{
		if (curr->value)
			printf("declare -x %s=\"%s\"\n", curr->key, curr->value);
		else if (!curr->value)
			printf("declare -x %s\n", curr->key);
		curr = curr->next;
	}
}

bool	check_valid_key_export(const char *key)
{
	int	i;

	if (!key || !key[0])
		return (g_zebi.g_exit = 1, false);
	if (!ft_isalpha(key[0]) && key[0] != '_')
		return (g_zebi.g_exit = 1, false);
	i = 1;
	while (key[i])
	{
		if (!ft_isalpha(key[i]) && key[i] != '_' && !ft_isdigit(key[i]))
		{
			ft_fprintf(2, "minishell: export: `%s': not a valid \
identifier\n", key);
			return (g_zebi.g_exit = 1, false);
		}
		i++;
	}
	return (true);
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

t_env	*env_create_sorted(t_env *env)
{
	t_env	*curr;
	t_env	*new;
	bool	swapped;

	if (!env)
		return (NULL);
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
