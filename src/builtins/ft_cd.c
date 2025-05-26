/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 23:02:42 by jd-halle          #+#    #+#             */
/*   Updated: 2025/05/27 01:22:36 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cd_error(char *r_p, char *o_p)
{
	free(r_p);
	free(o_p);
	g_exit = 1;
	perror("cd");
}

char	*if_no_rp(char *o_p)
{
	char	*r_p;

	ft_putstr_fd("cd: error retrieving current "
		"directory: getcwd: cannot access parent directories: "
		"No such file or directory\n", 2);
	r_p = malloc(ft_strlen(o_p) + 4);
	ft_strcpy(r_p, o_p);
	ft_strlcat(r_p, "/..", ft_strlen(o_p) + 4);
	return (r_p);
}

static char	*get_cd_path(t_token *tokens, t_env *env)
{
	char	*path;

	if (!tokens->next)
	{
		path = ft_getenv_value(env, "HOME");
		if (!path)
		{
			ft_putstr_fd("ministress: cd: HOME not set\n", 2);
			return (NULL);
		}
		return (path);
	}
	if (tokens->next->next)
	{
		g_exit = 1;
		ft_putstr_fd("cd: too many arguments\n", 2);
		return (NULL);
	}
	if (ft_strcmp(tokens->next->value, "-") == 0)
	{
		manage_cd_minus(env);
		return (NULL);
	}
	return (tokens->next->value);
}

static void	handle_cd_parent(t_env *env)
{
	char	*o_p;
	char	*r_p;

	o_p = ft_getenv_value(env, "PWD");
	if (chdir("..") == -1)
	{
		free(o_p);
		g_exit = 1;
		perror("cd");
		return ;
	}
	r_p = getcwd(NULL, 0);
	if (!r_p)
		r_p = if_no_rp(o_p);
	update_free_end_cd(env, o_p, r_p);
	g_exit = 0;
}

void	ft_cd(t_token *tokens, t_env *env)
{
	char	*path;
	char	*o_p;
	char	*r_p;

	path = get_cd_path(tokens, env);
	if (!path)
		return ;
	if (ft_strcmp(path, "..") == 0)
		return (handle_cd_parent(env));
	r_p = resolve_path(path);
	if (!r_p)
		return (g_exit = 1, perror("cd"));
	o_p = ft_getenv_value(env, "PWD");
	if (chdir(r_p) == -1)
		return (cd_error(r_p, o_p));
	update_free_end_cd(env, o_p, r_p);
	g_exit = 0;
}
