/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebonsushi <lebonsushi@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 23:02:42 by jd-halle          #+#    #+#             */
/*   Updated: 2025/05/20 14:44:00 by lebonsushi       ###   ########.fr       */
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

char	*ft_getenv_value(t_env *env, char *key)
{
	if (!env || !key)
		return (NULL);
	while (env)
	{
		if (!ft_strcmp(env->key, key))
			return (ft_strdup(env->value));
		env = env->next;
	}
	return (NULL);
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
		return (g_zebi.g_exit = 1, perror("cd"));
	}
	update_env(&env, "PWD", r_p, 1);
	update_env(&env, "OLDPWD", o_p, 1);
	free(o_p);
	free (r_p);
	g_zebi.g_exit = 0;
	return ;
}

void	update_free_end_cd(t_env *env, char *o_p, char *r_p)
{
	update_env(&env, "OLDPWD", o_p, 1);
	update_env(&env, "PWD", r_p, 1);
	free(o_p);
	free(r_p);
}

static void	cd_error(char *r_p, char *o_p)
{
	free(r_p);
	free(o_p);
	g_zebi.g_exit = 1;
	perror("cd");
}

void	ft_cd(t_token *tokens, t_env *env)
{
	t_token	*current;
	char	*path;
	char	*o_p;
	char	*r_p;

	current = tokens->next;
	if (!current)
	{
		path = ft_getenv_value(env, "HOME");
		if (!path)
			return (ft_putstr_fd("ministress: cd: HOME not set\n", 2));
		free(path);
	}
	else if (current && current->next)
		return (g_zebi.g_exit = 1, ft_putstr_fd("cd: too many \
arguments\n", 2));
	else if (ft_strcmp(current->value, "-") == 0)
		return (manage_cd_minus(env));
	r_p = resolve_path(current->value);
	if (!r_p)
		return (g_zebi.g_exit = 1, perror("cd"));
	o_p = ft_getenv_value(env, "PWD");
	if (chdir(r_p) == -1)
		return (cd_error(r_p, o_p));
	update_free_end_cd(env, o_p, r_p);
}
