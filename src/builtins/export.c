/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 15:43:10 by macorso           #+#    #+#             */
/*   Updated: 2025/01/27 18:53:28 by macorso          ###   ########.fr       */
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
	printf("key added: %s\n", key);
	printf("value added: %s\n", value);
}

void	export_builtin(t_all *all, char **strs)
{
	int		i;
	char	*key;
	char	*value;
	t_env	*env;

	i = -1;
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
