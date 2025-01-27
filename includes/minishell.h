/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 13:48:17 by macorso           #+#    #+#             */
/*   Updated: 2025/01/27 18:39:26 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

# define MINISHELL_H

# include "../libft/libft.h"

# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <unistd.h>

typedef enum e_token
{
	WORD,
	SEMICOLON,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
}	t_token;

typedef struct s_env
{
	char		*key; // name before the = sign
	char		*value; // name after the = sign
	struct s_env	*next;
}	t_env;

typedef struct s_all
{
	t_env	*env;
}	t_all;

void	print_header(void);

//env
char	*find_key(const char *envp_value);
char	*find_value(const char *envp_value);
char	*ft_getenv(t_all *all, char *key);
void	verify_addkey(t_all *all, char *key, char *value);

//init
void	init_all(t_all *all, int ac, char **av, char **envp);
t_env	*env_new(char *key, char *value);
t_env	*env_get_last(t_env	*env);
void	env_addback(t_env **env, t_env *new);

//destroy
void	free_env(t_env *env);
void	free_all_env(t_all *all);

//builtins
void	env_builtin(t_all *all);
void	export_builtin(t_all *all, char **keys);
void	unset_builtin(t_all *all, char **keys);
void	echo_builtin(char **strs, bool n_option);
void	pwd_builtin(void);

#endif