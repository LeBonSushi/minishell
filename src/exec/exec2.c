/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:50:22 by jd-halle          #+#    #+#             */
/*   Updated: 2025/05/07 18:01:48 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	is_file(char *cmd)
{
	int	fd;

	fd = open(cmd, O_RDONLY);
	if (fd >= 0)
	{
		close(fd);
		return (true);
	}
	return (false);
}

void	exec_error(t_all *all, char *cmd)
{
	(void)all;
	if (errno == EACCES && g_exit == 0 && !is_file(cmd) && ft_strcmp(cmd,
			""))
	{
		ft_fprintf(2, "%s: Permission denied\n", cmd);
		g_exit = 126;
		return ;
	}
	else if ((!ft_strncmp(cmd, "./", 2) || !ft_strncmp(cmd, "/", 1))
		&& g_exit == 0)
	{
		perror(cmd);
		// printf("oausi\n");
		if (errno != ENOENT)
		{
			g_exit = 126;
			return ;
		}
	}
	else
		ft_fprintf(2, "%s: command not found\n", cmd);
	g_exit = 127;
}

char	*get_cmd_path(t_all *all, char *cmd, char **env)
{
	char	**paths;
	char	*exe_path;

	if (!cmd)
		return (NULL);
	if (!access(cmd, X_OK) && !is_dir(all, cmd))
		return (ft_strdup(cmd));
	if (!*env)
	{
		perror(cmd);
		return (NULL);
	}
	paths = get_paths(all, env);
	exe_path = find_exec_path(cmd, paths);
	free_tab(paths);
	return (exe_path);
}

void	pipe_error(t_all *all, int *fd)
{
	(void)all;
	close(fd[0]);
	close(fd[1]);
	perror("pipe");
	g_exit = 1;
}

void	fork_error(t_all *all, int *fd)
{
	(void)all;
	close(fd[0]);
	close(fd[1]);
	perror("fork");
	g_exit = 1;
}
