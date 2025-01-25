/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_prepare.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 14:53:14 by macorso           #+#    #+#             */
/*   Updated: 2025/01/25 18:17:28 by macorso          ###   ########.fr       */
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

t_env	*env_last(t_env	*env)
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
		tmp = *env;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
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
	if (!envp_value[start + 1])
		return (NULL);
	value = ft_substr(envp_value, start + 1, end);
	if (!value)
		return (NULL);
	return (value);
}

// a function that return a t_env* linked list
t_env	*fill_env(char **envp)
{
	t_env	*env;
	int		i;

	i = 0;
	env = NULL;
	while (envp[i])
	{
		env_addback(&env, env_new(find_key(envp[i]), find_value(envp[i])));
		i++;
	}
	return (env);
}

void	init_all(t_all *all, int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;

	all->env = fill_env(envp);
}
