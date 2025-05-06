/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 05:18:28 by macorso           #+#    #+#             */
/*   Updated: 2025/05/06 07:07:38 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	tokens_size(t_token *tokens)
{
	t_token	*curr;
	int		i;

	curr = tokens;
	i = 0;
	while (curr)
	{
		i++;
		curr = curr->next;
	}
	return (i);
}

char	**tokens_to_tab(t_token *tokens)
{
	char	**tab;
	t_token	*curr;
	int		i;

	if (!tokens)
		return (NULL);
	curr = tokens;
	tab = malloc(sizeof(char *) * (tokens_size(tokens) + 1));
	if (!tab)
		return (NULL);
	i = 0;
	while (curr)
	{
		tab[i] = ft_strdup(curr->value);
		i++;
		curr = curr->next;
	}
	tab[i] = NULL;
	return (tab);
}

char	**get_paths(t_all *all, char **env)
{
	int		i;
	char	**paths;

	i = 0;
	paths = NULL;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
		{
			paths = ft_split(env[i] + 5, ':');
			break ;
		}
		i++;
	}
	if (!paths)
	{
		ft_fprintf(2, "$PATH not found in env.\n");
		all->status = 127;
		return (NULL);
	}
	return (paths);
}

char	*find_exec_path(char *cmd, char **paths)
{
	int		i;
	char	*exec_path;
	char	*tmp;

	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/", 0, 0);
		exec_path = ft_strjoin(tmp, cmd, 1, 0);
		if (!access(exec_path, X_OK))
			return (exec_path);
		free(exec_path);
		exec_path = NULL;
		i++;
	}
	return (ft_strdup(cmd));
}

bool	is_dir(t_all *all, char *cmd)
{
	int		fd;
	char	*error;

	fd = open(cmd, O_RDONLY);
	error = ft_strjoin("ministress: ", cmd, 0, 0);
	if (fd < 0 && errno == EISDIR)
	{
		perror(error);
		all->status = 1;
	}
	free(error);
	close(fd);
	return (false);
}
