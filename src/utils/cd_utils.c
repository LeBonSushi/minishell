/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 01:11:29 by jd-halle          #+#    #+#             */
/*   Updated: 2025/05/27 01:22:36 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*resolve_path(const char *path)
{
	char	*cwd;
	char	*abs_path;
	int		cwd_len;
	int		path_len;

	if (path[0] == '/')
		return (ft_strdup(path));
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("getcwd");
		return (NULL);
	}
	cwd_len = ft_strlen(cwd);
	path_len = ft_strlen(path);
	abs_path = malloc(cwd_len + path_len + 2);
	if (!abs_path)
		return (perror("malloc"), free(cwd), NULL);
	ft_strcpy(abs_path, cwd);
	abs_path[cwd_len] = '/';
	ft_strcpy(abs_path + cwd_len + 1, path);
	free(cwd);
	return (abs_path);
}

void	manage_cd_minus(t_env *env)
{
	char	*path;
	char	*o_p;
	char	*r_p;

	path = ft_getenv_value(env, "OLDPWD");
	if (!path)
		return (ft_putstr_fd("ministress: cd: OLDPWD not set\n", 2));
	r_p = resolve_path(path);
	free(path);
	if (!r_p)
		return (perror("cd"));
	o_p = ft_getenv_value(env, "PWD");
	if (chdir(r_p) == -1)
	{
		free(r_p);
		return (g_exit = 1, perror("cd"));
	}
	update_env(&env, "PWD", r_p, 1);
	update_env(&env, "OLDPWD", o_p, 1);
	free(o_p);
	free (r_p);
	g_exit = 0;
	return ;
}

void	update_free_end_cd(t_env *env, char *o_p, char *r_p)
{
	update_env(&env, "OLDPWD", o_p, 1);
	update_env(&env, "PWD", r_p, 1);
	free(o_p);
	free(r_p);
}
