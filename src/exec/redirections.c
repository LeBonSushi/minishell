/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jd-halle <jd-halle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 02:40:39 by macorso           #+#    #+#             */
/*   Updated: 2025/04/29 22:01:44 by jd-halle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	env_get_size(t_env *env)
{
	t_env	*curr;
	int		i;

	curr = env;
	i = 0;
	while (curr)
	{
		i++;
		curr = curr->next;
	}
	return (i);
}

int	cmd_get_size(t_cmd *cmd)
{
	t_cmd	*curr;
	int		i;

	curr = cmd;
	i = 0;
	while (curr)
	{
		i++;
		curr = curr->next;
	}
	return (i);
}

char	**env_to_tab(t_env *env)
{
	char	**env_tab;
	t_env	*curr;
	int		i;

	curr = env;
	env_tab = malloc(sizeof(char *) * (env_get_size(env) + 1));
	if (!env_tab)
		return (NULL);
	i = 0;
	while (curr)
	{
		env_tab[i] = ft_strjoin(curr->key, "=", 0, 0);
		env_tab[i] = ft_strjoin(env_tab[i], curr->value, 1, 0);
		i++;
		curr = curr->next;
	}
	env_tab[i] = NULL;
	return (env_tab);
}

char	**cmd_to_tab(t_cmd *cmd)
{
	char	**cmd_tab;
	t_cmd	*curr;
	int		i;

	curr = cmd;
	cmd_tab = malloc(sizeof (char *) * (cmd_get_size(cmd) + 1));
	i = 0;
	if (!cmd_tab)
		return (NULL);
	while (curr)
	{
		cmd_tab[i] = ft_strdup(curr->tokens->value);
		i++;
		curr = curr->next;
	}
	cmd_tab[i] = NULL;
	return (cmd_tab);
}

bool	ft_compare(char *limiter, char *line)
{
	size_t	limiter_len;
	size_t	line_len;

	if (!line || !limiter)
		return (0);
	limiter_len = ft_strlen(limiter);
	line_len = ft_strlen(line);
	if (line_len != limiter_len)
		return (0);
	if (ft_strncmp(line, limiter, limiter_len))
		return (0);
	return (1);
}
