/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 15:43:10 by macorso           #+#    #+#             */
/*   Updated: 2025/05/27 01:22:36 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		return (g_exit = 1, 1);
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
	return (0);
}

static int	handle_var_without_value(t_env **env, const char *token_value)
{
	char	*key;

	key = find_key(token_value);
	if (!key || !check_valid_key_export(key))
	{
		g_exit = 1;
		free(key);
		return (1);
	}
	if (!find_env_var(*env, key))
	{
		env_addback(env, env_new(key, NULL));
		free(key);
	}
	else
		free(key);
	return (0);
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
		if (ft_strchr(current->value, '='))
			g_exit = handle_var_with_value(env, current->value);
		else
			g_exit = handle_var_without_value(env, current->value);
		current = current->next;
	}
}
