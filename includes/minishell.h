/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 13:48:17 by macorso           #+#    #+#             */
/*   Updated: 2025/01/28 05:23:18 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

# define MINISHELL_H

# include "../libft/libft.h"

# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef enum e_token_type
{
	WORD = 1,
	SEMICOLON,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC,
}	t_token_type;

typedef enum e_node_type
{
	CMD = 1,
	OPERATOR_PIPE,
	OPERATOR_SEMICOLON,
	OPERATOR_AND,
	OPERATOR_OR,
}	t_node_type;

typedef struct s_env
{
	char		*key; // name before the = sign
	char		*value; // name after the = sign
	struct s_env	*next;
}	t_env;


typedef struct s_token
{
	t_token_type	token_type;
	struct s_token	*next;
}	t_token;



typedef struct s_tree
{
	struct s_tree	*left_child;
	struct s_tree	*right_child;
	t_node_type		node_type;
	t_string_list		*args;
	t_string_list		*redir_in_names;
	t_string_list		*redir_out_names;
}	t_tree;

typedef struct s_string_list
{
	char	*str;
	struct s_string_list	*next;
}	t_string_list;

typedef struct s_all
{
	t_env	*env;
	t_tree	*tree;
}	t_all;

void	print_header(void);

//---------------------ENV----------------------

char	*find_key(const char *envp_value);
char	*find_value(const char *envp_value);
char	*ft_getenv(t_all *all, char *key);
void	verify_addkey(t_all *all, char *key, char *value);
t_env	*env_create_sorted(t_env *env);

//---------------------INIT----------------------

void	init_all(t_all *all, int ac, char **av, char **envp);
t_env	*env_new(char *key, char *value);
t_env	*env_get_last(t_env	*env);
void	env_addback(t_env **env, t_env *new);

//---------------------DESTROY----------------------

void	free_env(t_env *env);
void	free_all_env(t_env *env);

//--------------------BUILTINS----------------------

void	env_builtin(t_all *all);
void	export_builtin(t_all *all, char **keys);
void	unset_builtin(t_all *all, char **keys);
void	echo_builtin(char **strs, bool n_option);
void	pwd_builtin(void);

//---------------------PARSING----------------------

void	minishell_loop(t_all *all);
void	print_token(t_token	*token);
char	**split_2_sep(const char *str, char sep, char *seplist);
t_command	*parse_str_to_command(char *str);
t_token	get_token(char c);
void	print_tab(char **tab);
char	**split_str_token(char *str, char *token_str);
char	*str_to_token(char *str);

#endif