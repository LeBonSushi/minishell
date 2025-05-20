/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebonsushi <lebonsushi@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:50:22 by jd-halle          #+#    #+#             */
/*   Updated: 2025/05/20 14:42:58 by lebonsushi       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

// void	exec_error(t_all *all, char *cmd)
// {
// 	(void)all;
// 	if (errno == EACCES && g_zebi.g_exit == 0 && !is_file(cmd) && ft_strcmp(cmd,
// 			""))
// 	{
// 		ft_fprintf(2, "%s: Permission denied\n", cmd);
// 		g_zebi.g_exit = 126;
// 		return ;
// 	}
// 	else if ((!ft_strncmp(cmd, "./", 2) || !ft_strncmp(cmd, "/", 1))
// 		&& g_zebi.g_exit == 0)
// 	{
// 		perror(cmd);
// 		if (errno != ENOENT)
// 		{
// 			g_zebi.g_exit = 126;
// 			return ;
// 		}
// 	}
// 	else
// 		ft_fprintf(2, "%s: command not found\n", cmd);
// 	g_zebi.g_exit = 127;
// }

void	exec_error(char *cmd)
{
	struct stat	file_stat;
	int			stat_result;
	
	if (!cmd || !*cmd)
	{
		ft_fprintf(2, "ministress: : command not found\n");
		g_zebi.g_exit = 127;
		return ;
	}
	
	// Vérifier si le fichier existe
	stat_result = stat(cmd, &file_stat);
	
	// Gestion des chemins absolus et relatifs (./, ../, /)
	if (ft_strncmp(cmd, "./", 2) == 0 || ft_strncmp(cmd, "../", 3) == 0 || 
		ft_strncmp(cmd, "/", 1) == 0)
	{
		if (stat_result == 0 && S_ISDIR(file_stat.st_mode))
		{
			ft_fprintf(2, "ministress: %s: Is a directory\n", cmd);
			g_zebi.g_exit = 126;
			return ;
		}
		else if (stat_result == 0 && !(file_stat.st_mode & S_IXUSR))
		{
			ft_fprintf(2, "ministress: %s: Permission denied\n", cmd);
			g_zebi.g_exit = 126;
			return ;
		}
		else if (stat_result != 0)
		{
			ft_fprintf(2, "ministress: %s: No such file or directory\n", cmd);
			g_zebi.g_exit = 127;
			return ;
		}
	}
	// Gestion des noms de fichiers simples (sans ./ ou ../ ou /)
	else
	{
		// Vérifier si le fichier existe mais n'est pas dans le PATH
		if (stat_result == 0)
		{
			// Si c'est un répertoire dans le répertoire courant
			if (S_ISDIR(file_stat.st_mode))
			{
				ft_fprintf(2, "ministress: %s: command not found\n", cmd);
				g_zebi.g_exit = 127;
				return ;
			}
			else
			{
				ft_fprintf(2, "ministress: %s: command not found\n", cmd);
				g_zebi.g_exit = 127;
				return ;
			}
		}
		else
		{
			ft_fprintf(2, "ministress: %s: command not found\n", cmd);
			g_zebi.g_exit = 127;
			return ;
		}
	}
	
	// Par défaut, commande non trouvée
	ft_fprintf(2, "ministress: %s: command not found\n", cmd);
	g_zebi.g_exit = 127;
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
	exe_path = find_exec_path(cmd, paths, paths);
	free_tab(paths);
	return (exe_path);
}

void	pipe_error(t_all *all, int *fd)
{
	(void)all;
	close(fd[0]);
	close(fd[1]);
	perror("pipe");
	g_zebi.g_exit = 1;
}

void	fork_error(t_all *all, int *fd)
{
	(void)all;
	close(fd[0]);
	close(fd[1]);
	perror("fork");
	g_zebi.g_exit = 1;
}
