/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jd-halle <jd-halle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 15:43:10 by macorso           #+#    #+#             */
/*   Updated: 2025/02/26 09:02:39 by jd-halle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	find_egal_in_token(char *env)
{
	int i;

	i = 0;
	while (env[i])
	{
		if (env[i] == '=')
			return (true);
		i++;
	}
	return (false);
}

void	print_env(t_env *env)
{
	t_env *curr = env;
	while (curr)
	{
		if (curr->value)
			printf("declare -x %s=\"%s\"\n", curr->key, curr->value);
		else if (!curr->value)
			printf("declare -x %s\n", curr->key);
		curr = curr->next;
	}
}

t_env	*env_find(t_env *env, char *key)
{
	t_env *curr = env;
	
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
	char *key;
	char *value;
	t_env *existing;
	t_token *current;
	t_env *env_sorted = *env;

	if (!tokens || !env || !(*env))
		return;
	current = tokens->next;
	if (!current)
	{
		env_sorted = env_create_sorted(env_sorted);
		return (print_env (env_sorted));                      //no args apres export
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
				continue;
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
				continue;
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

bool check_valid_key_export(const char *key)
{
	int i;

	if (!key || !key[0])
		return (false);
	if (!('a' <= key[0] && key[0] <= 'z') && !('A' <= key[0] && key[0] <= 'Z') && key[0] != '_')
	{
		//ft_putstr_fd("invalid start key name\n", 2);
		return (false);
	}
	i = 1;
	while (key[i])
	{
		if (!('a' <= key[i] && key[i] <= 'z') && !('A' <= key[i] && key[i] <= 'Z') && key[i] != '_' && !('0' <= key[i] && key[i] <= '9'))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd((char*)key, 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
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
