/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_errors.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:54:36 by macorso           #+#    #+#             */
/*   Updated: 2025/05/26 22:22:42 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_perm(struct stat file_stat, int stat_result, char *cmd)
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
	stat_result = stat(cmd, &file_stat);
	if (ft_strncmp(cmd, "./", 2) == 0 || ft_strncmp(cmd, "../", 3) == 0
		|| ft_strncmp(cmd, "/", 1) == 0)
		return (check_perm(file_stat, stat_result, cmd));
	ft_fprintf(2, "ministress: %s: command not found\n", cmd);
	g_zebi.g_exit = 127;
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
