/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jd-halle <jd-halle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 15:27:48 by macorso           #+#    #+#             */
/*   Updated: 2025/05/27 01:12:46 by jd-halle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_pwd(t_env *env)
{
	char	*cwd;
	char	*pwd_env;

	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		ft_putendl_fd(cwd, 1);
		free(cwd);
	}
	else
	{
		pwd_env = ft_getenv_value(env, "PWD");
		if (pwd_env)
		{
			ft_putendl_fd(pwd_env, 1);
			free(pwd_env);
		}
		else
			perror("pwd");
	}
}
