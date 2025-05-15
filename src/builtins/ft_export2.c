/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jd-halle <jd-halle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 22:10:59 by jd-halle          #+#    #+#             */
/*   Updated: 2025/05/15 10:58:51 by jd-halle         ###   ########.fr       */
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
	{
		g_zebi.g_exit = 1;
		return (false);
	}
	if (!('a' <= key[0] && key[0] <= 'z') && !('A' <= key[0]
			&& key[0] <= 'Z') && key[0] != '_')
	{
		g_zebi.g_exit = 1;
		return (false);
	}
	i = 1;
	while (key[i])
	{
		if (!('a' <= key[i] && key[i] <= 'z') && !('A' <= key[i]
				&& key[i] <= 'Z') && key[i] != '_'
			&& !('0' <= key[i] && key[i] <= '9'))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd((char *)key, 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			g_zebi.g_exit = 1;
			return (false);
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
