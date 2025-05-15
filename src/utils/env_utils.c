/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jd-halle <jd-halle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 00:17:58 by macorso           #+#    #+#             */
/*   Updated: 2025/05/15 04:34:49 by jd-halle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


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

void	update_env(t_env **env, char *name, char *value, bool overwrite)
{
	int len;
	t_env *tmp;
	t_env *new_node;
	
	if (!name || !value)
		return;
	len = ft_strlen(name);
	tmp = *env;
	while (tmp)
	{
		if (ft_strncmp(tmp->key, name, len) == 0 && tmp->key[len] == '\0')
		{
			if (!overwrite)
				return;
			free(tmp->value);
			tmp->value = ft_strdup(value);
			return ;
		}
		tmp = tmp->next;
	}
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->key = ft_strdup(name);
	new_node->value = ft_strdup(value);
	new_node->next = NULL;
	if (!(*env))
	{
		*env = new_node;
		return ;
	}
	env_addback(env, new_node);
	// ajouter un sort env; /////////////////////////////////////////////////////////
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