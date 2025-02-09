/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_prepare.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 14:53:14 by macorso           #+#    #+#             */
/*   Updated: 2025/02/07 13:13:58 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_env	*env_new(char *key, char *value)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = ft_strdup(key);
	new->value = ft_strdup(value);
	new->next = NULL;
	return (new);
}

t_env	*env_get_last(t_env	*env)
{
	t_env	*last;

	if (!env)
		return (NULL);
	last = env;
	while (last->next)
		last = last->next;
	return (last);
}

void	env_addback(t_env **env, t_env *new)
{
	t_env	*tmp;

	if (!*env)
		*env = new;
	else
	{
		tmp = env_get_last(*env);
		tmp->next = new;
	}
}

bool check_valid_key(const char *str)
{
	int i;

	i = 0;
	if (str == NULL)
		return (false);
	if (!('a' <= str[i] && str[i] <= 'z') && !('A' <= str[i] && str[i] <= 'Z') && str[i] != '_')
		return (false);

	return (true);
}

char	*find_key(const char *envp_value)
{
	int		end;
	char	*key;

	end = 0;
	if (!envp_value)
		return (NULL);
	while(envp_value[end] != '=' && envp_value[end])
		end++;
	key = ft_substr(envp_value, 0, end);
	if (!key)
		return (ft_putstr_fd("Malloc failed\n", 2), NULL);
	if (!check_valid_key(key))
	{
		free(key);
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd((char *)envp_value, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (NULL);
	}
	return (key);
}

char	*find_value(const char *envp_value)
{
	int		start;
	int		end;
	char	*value;
	
	if (!envp_value)
		return (NULL);
	end = ft_strlen(envp_value);
	if (!end)
		return (NULL);
	start = 0;
	while (envp_value[start] && envp_value[start] != '=')
		start++;
	if (envp_value[start] != '=')
		return (NULL);
	if (!envp_value[start + 1])
		return (NULL);
	value = ft_substr(envp_value, start + 1, end);
	if (!value)
		return (NULL);
	return (value);
}

char	*ft_getenv(t_all *all, char *key)
{
	t_env	*env;
	
	env = all->env;
	while (env)
	{
		if (!ft_strcmp(env->key, key))
			return (ft_strdup(env->value));
		env = env->next;
	}
	return (NULL);
}

// a function that return a t_env* linked list
t_env	*fill_env(char **envp)
{
	t_env	*env;
	int		i;
	char	*key;
	char	*value;

	i = 0;
	env = NULL;
	while (envp[i])
	{
		key = find_key(envp[i]);
		value = find_value(envp[i]);
		env_addback(&env, env_new(key, value));
		free(key);
		free(value);
		i++;
	}
	return (env);
}
