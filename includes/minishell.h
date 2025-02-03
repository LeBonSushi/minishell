/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 13:48:17 by macorso           #+#    #+#             */
/*   Updated: 2025/02/03 14:38:44 by macorso          ###   ########.fr       */
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
	char			*key; // name before the = sign
	char			*value; // name after the = sign
	struct s_env	*next;
}	t_env;


typedef struct s_token
{
	t_token_type	token_type;
	bool			single_quoted;
	bool			double_quoted;
	char			*value;
	struct s_token	*next;
}	t_token;

typedef struct s_string_list
{
	char	*str;
	struct s_string_list	*next;
}	t_string_list;

typedef struct s_tree
{
	t_node_type		node_type;
	struct s_tree	*left;
	struct s_tree	*right;
	t_string_list	*args;
	t_string_list	*redir_in_names;
	t_string_list	*redir_out_names;
}	t_tree;

typedef struct s_all
{
	t_env	*env;
	t_token	*tokens;
	t_tree	*tree;
	int		status;
}	t_all;

void	print_header(void);
void	print_tab(char **tab);

//---------------------ENV-------------------------------

t_env	*env_new(char *key, char *value);
t_env	*env_get_last(t_env	*env);
void	env_addback(t_env **env, t_env *new);
char	*find_key(const char *envp_value);
char	*find_value(const char *envp_value);
char	*ft_getenv(t_all *all, char *key);
void	verify_addkey(t_all *all, char *key, char *value);
t_env	*env_create_sorted(t_env *env);

//---------------------TOKENS----------------------------

t_token	*token_new(t_token_type token_type, char *value, bool single_q, bool double_q);
void	token_addback(t_token **lst, t_token *new);
t_token	*fill_tokens(char *str);
void	print_tokens(t_token *tokens);

//---------------------INIT------------------------------

void	init_all(t_all *all, int ac, char **av, char **envp);
t_env	*fill_env(char **envp);

//---------------------DESTROY---------------------------

void	free_env(t_env *env);
void	free_all_env(t_env *env);
void	free_all_token(t_token *token);
void	free_all(t_all *all);

//--------------------BUILTINS---------------------------

void	env_builtin(t_all *all);
void	export_builtin(t_all *all, char **keys);
void	unset_builtin(t_all *all, char **keys);
void	echo_builtin(char **strs, bool n_option);
void	pwd_builtin(void);
char	*pwd_builtin_char();

//---------------------PARSING---------------------------

void	minishell_loop(t_all *all);

#endif