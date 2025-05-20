/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebonsushi <lebonsushi@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 15:43:10 by macorso           #+#    #+#             */
/*   Updated: 2025/05/20 11:07:20 by lebonsushi       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	contains_equal_sign(const char *str)
{
	if (!str)
		return (false);
	return (ft_strchr(str, '=') != NULL);
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
	if (!key || !check_valid_key_export(key))
	{
		if (key)
			free(key);
		return (1);
	}
	value = find_value(token_value);
	existing = getnode_env_value(*env, key);
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
		if (key)
			free(key);
		return (1);
	}
	if (!getnode_env_value(*env, key))
		env_addback(env, env_new(key, NULL));
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
		return (print_sorted_env(*env));
	while (current)
	{
		if (contains_equal_sign(current->value))
			handle_var_with_value(env, current->value);
		else
			handle_var_without_value(env, current->value);
		current = current->next;
	}
}
