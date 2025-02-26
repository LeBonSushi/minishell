#include "../../includes/minishell.h"

char *resolve_path(const char *path)
{
	char	*cwd;
	char	*abs_path;
	int		cwd_len;
	int		path_len;

	if (path[0] == '/')
		return ft_strdup(path);
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("getcwd");
		return NULL;
	}
	cwd_len = ft_strlen(cwd);
	path_len = ft_strlen(path);
	abs_path = malloc(cwd_len + path_len + 2);
	if (!abs_path)
		return(perror("malloc"), free(cwd), NULL);
	ft_strcpy(abs_path, cwd);
	abs_path[cwd_len] = '/';
	ft_strcpy(abs_path + cwd_len + 1, path);
	free(cwd);
	return (abs_path);
}
char	*ft_getenv_value(t_env *env, char *key)
{
	while (env)
	{
		if (!ft_strcmp(env->key, key))
			return (ft_strdup(env->value));
		env = env->next;
	}
	return (NULL);
}

void	ft_cd(t_token *tokens, t_env *env)
{
	t_token	*current;
	char	*path;
	char	*old_pwd;
	char	*resolved_path;
	
	current = tokens->next;
	if (!current)
	{
		path = ft_getenv_value(env, "HOME");
		if (!path)
			return (ft_putstr_fd("cd: HOME not set\n", 2));
	}
	else if (current && current->next)
		return(ft_putstr_fd("cd: too many arguments\n", 2));
	else if (ft_strcmp(current->value, "-") == 0)
	{
		path = ft_getenv_value(env, "OLDPWD");
		if (!path)
			return (ft_putstr_fd("cd: OLDPWD not set\n", 2));
		ft_putendl_fd(path, 1);
		resolved_path = resolve_path(path);
		if (!resolved_path)
			return(perror("cd"));
		old_pwd = ft_getenv_value(env, "PWD");
		if (chdir(resolved_path) == -1)
		{
			free(resolved_path);
			return (perror("cd"));
		}
		update_env(&env, "PWD", resolved_path, 1);
		update_env(&env, "OLDPWD", old_pwd, 1);
		free(old_pwd);
		free (resolved_path);
		return;
	}
	else
		path = current->value;
	resolved_path = resolve_path(path);
	if (!resolved_path)
		return (perror("cd"));
	old_pwd = ft_getenv_value(env, "PWD");
	if (!old_pwd)
		old_pwd = NULL;
	if (chdir(resolved_path) == -1)
	{
		free(resolved_path);
		perror("cd");
		free(old_pwd);
		return;
	}
	update_env(&env, "OLDPWD", old_pwd, 1);
	update_env(&env, "PWD", resolved_path, 1);
	free(old_pwd);
	free(resolved_path);
}
