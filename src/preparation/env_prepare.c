/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_prepare.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 14:53:14 by macorso           #+#    #+#             */
/*   Updated: 2025/01/24 17:10:08 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_env	*env_new(char *key, char *value)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = key;
	new->value = value;
	new->have_equal = true;
	new->next = NULL;
	return (new);
}

void	env_addback(t_env *env, t_env *new)
{
	t_env	*tmp;

	if (!env)
		return ;
	tmp = env;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

char	*find_key(char *envp_value)
{
	int		end;
	char	*key;

	end = 0;
	if (!envp_value)
		return (NULL);
	while(envp_value[end] != '=' && envp_value[end])
		end++;
	if (envp_value[end] != '=')
		return (NULL);
	key = ft_substr(envp_value, 0, end);
	if (!key)
		return (NULL);
	return (key);
}

char	*find_value(char *envp_value)
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
	value = ft_substr(envp_value, start + 1, end);
	if (!value)
		return (NULL);
	return (value);
}

void	fill_env(t_env *env, char **envp)
{
	int		i;
	char	*key;
	char	*value;

	i = 0;
	while (envp[i])
	{
		key = find_key(envp[i]);
		value = find_value(envp[i]);
		env_addback(env, env_new(key, value));
		free(key);
		free(value);
		i++;
	}
}
