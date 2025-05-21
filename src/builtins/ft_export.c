/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 15:43:10 by macorso           #+#    #+#             */
/*   Updated: 2025/05/21 11:04:30 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// bool	find_egal_in_token(char *env)
// {
// 	int	i;

// 	i = 0;
// 	while (env[i])
// 	{
// 		if (env[i] == '=')
// 			return (true);
// 		i++;
// 	}
// 	return (false);
// }

// t_env	*env_find(t_env *env, char *key)
// {
// 	t_env	*curr;

// 	curr = env;
// 	while (curr)
// 	{
// 		if (ft_strcmp(curr->key, key) == 0)
// 			return (curr);
// 		curr = curr->next;
// 	}
// 	return (NULL);
// }

// void	ft_export(t_token *tokens, t_env **env)
// {
// 	char	*key;
// 	char	*value;
// 	t_env	*existing;
// 	t_token	*current;
// 	t_env	*env_sorted;

// 	env_sorted = *env;
// 	if (!tokens || !env || !(*env))
// 		return ;
// 	current = tokens->next;
// 	if (!current)
// 	{
// 		env_sorted = env_create_sorted(env_sorted);
// 		print_env(env_sorted);
// 		free_all_env(env_sorted);
// 		return ;
// 	}
// 	while (current)
// 	{
// 		if (find_egal_in_token(current->value))
// 		{
// 			key = find_key(current->value);
// 			value = find_value(current->value);
// 			if (!check_valid_key_export(key))
// 			{
// 				free(key);
// 				free(value);
// 				current = current->next;
// 				continue ;
// 			}
// 			existing = env_find(*env, key);
// 			free(key);
// 			free(value);
// 			if (existing)
// 			{
// 				if (existing->value)
// 					free(existing->value);
// 				existing->value = value;
// 				free(key);
// 			}
// 			else
// 				env_addback(env, env_new(key, value));
// 		}
// 		else
// 		{
// 			key = find_key(current->value);
// 			if (!check_valid_key_export(current->value))
// 			{
// 				free(key);
// 				current = current->next;
// 				continue ;
// 			}
// 			if (!env_find(*env, key))
// 			{
// 				env_addback(env, env_new(key, NULL));
// 				free(key);
// 			}
// 			else
// 				free(key);
// 		}
// 		current = current->next;
// 	}
// 	env_sorted = env_create_sorted(env_sorted);
// 	free_all_env(env_sorted);
// }


static bool	contains_equal_sign(const char *str)
{
	if (!str)
		return (false);
	return (ft_strchr(str, '=') != NULL);
}

static t_env	*find_env_var(t_env *env, const char *key)
{
	if (!key)
		return (NULL);
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

static void	update_existing_var(t_env *existing, char *new_value)
{
	if (!existing)
		return ;
	if (existing->value)
		free(existing->value);
	existing->value = new_value;
}

static int	handle_var_with_value(t_env **env, const char *token_value)
{
	char	*key;
	char	*value;
	t_env	*existing;

	key = find_key(token_value);
	value = find_value(token_value);
	if (!key || !check_valid_key_export(key))
	{
		free(key);
		free(value);
		return (1);
	}
	existing = find_env_var(*env, key);
	if (existing)
	{
		update_existing_var(existing, value);
		free(key);
	}
	else
	{
		env_addback(env, env_new(key, value));
		free(key);
		free(value);
	}
	return (0);  // Success
}

static int	handle_var_without_value(t_env **env, const char *token_value)
{
	char	*key;

	key = find_key(token_value);
	if (!key || !check_valid_key_export(key))
	{
		free(key);
		return (1);
	}
	if (!find_env_var(*env, key))
	{
		env_addback(env, env_new(key, NULL));
		free(key);
	}
	else
	{
		free(key);
	}
	return (0);
}

static void	print_sorted_env(t_env *env)
{
	t_env	*sorted_env;

	sorted_env = env_create_sorted(env);
	if (sorted_env)
	{
		print_env(sorted_env);
		free_all_env(sorted_env);
	}
}

void	ft_export(t_token *tokens, t_env **env)
{
	t_token	*current;

	if (!tokens || !env || !(*env))
		return ;
	current = tokens->next;
	
	if (!current)
	{
		print_sorted_env(*env);
		return ;
	}
	
	while (current)
	{
		if (contains_equal_sign(current->value))
			g_zebi.g_exit = handle_var_with_value(env, current->value);
		else
			g_zebi.g_exit = handle_var_without_value(env, current->value);
		current = current->next;
	}
}