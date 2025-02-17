/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 13:48:17 by macorso           #+#    #+#             */
/*   Updated: 2025/02/05 03:29:14 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

# define MINISHELL_H

# include "../libft/libft.h"

# include <stdio.h>
# include <dirent.h>
# include <stdlib.h>
# include <stdbool.h>
# include <string.h>
# include <errno.h>
# include <strings.h>
# include <unistd.h>
# include <sys/wait.h>
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
	WILD_CARD,
	OR,
	AND,
	PARENTHESIS_OPEN,
	PARENTHESIS_CLOSE,
	TILDE,
}	t_token_type;

typedef enum e_redir_type
{
	TYPE_REDIR_IN = 1,
	TYPE_REDIR_OUT,
	TYPE_REDIR_APPEND,
	TYPE_REDIR_HEREDOC,
}	t_redir_type;

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
	bool			space_after;
	bool			expand_wildcard;
	char			*value;
	struct s_token	*next;
}	t_token;

typedef struct s_string_list
{
	char				*str;
	t_redir_type		redir_type;
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
	pid_t	pid;
	int		status;
}	t_all;

void	print_header(void);
void	print_tab(char **tab);
char	**from_list_to_tab(t_env	*list);

//---------------------ENV-------------------------------


void	ft_stradd_back(t_string_list **lst, t_string_list *new);
t_string_list	*ft_strnew(char *content, t_redir_type redir_type);
t_string_list	*ft_strlast(t_string_list *lst);
void	free_str_list(t_string_list *lst);

t_env	*env_new(char *key, char *value);
t_env	*env_get_last(t_env	*env);
void	env_addback(t_env **env, t_env *new);
char	*find_key(const char *envp_value);
char	*find_value(const char *envp_value);
char	*ft_getenv(t_all *all, char *key);
void	verify_addkey(t_all *all, char *key, char *value);
t_env	*env_create_sorted(t_env *env);

//---------------------TOKENS----------------------------

char		*from_tokens_to_str(t_token *tokens);
bool			is_operator(char *str);
bool			is_quote(char c);
bool			is_quotes_closed(char *str);
bool			is_valid_tokens(t_all *all, t_token *token);
void			delete_token(t_token **tokens, t_token *token_to_del);
void			edit_token(t_token **token, t_token_type type, char *value);
t_token			*token_new(t_token_type token_type, char *value, bool single_q, bool double_q);
void			token_addback(t_token **lst, t_token *new);
void			manage_empty_tokens(t_token **tokens);
t_token			*ft_parse_quoted(char **str, bool *sq, bool *dq);
t_token			*ft_parse_word(char **str, bool sq, bool dq);
t_token			*ft_parse_operator(char **str);
t_token			*new_parsed_token(char	**str);
t_token			*fill_tokens(char *str, t_all *all);
t_token			*copy_token(t_token *tmp, t_token **next_ptr);
t_token			*join_tokens(t_token *tmp, t_token *next, t_token **next_ptr);
t_token			*merge_tokens(t_token *tokens);

void			print_tokens(t_token *tokens);

//---------------------EXPANDER--------------------------
bool	is_expandable_wildcard(t_token *token);
bool	is_hidden_valid(char *str, char *pattern);
void	sort_matches(char **matches, int count);
t_token	*create_tokens_from_matches(char **matches,
				t_token *to_expand);
char	**store_matches(DIR *dir, t_token *to_expand, int *match_count);
void	skip_variable_chars(char *word, int *i, int start);
void	handle_dollar_sign(t_all *all, char *word, int *i,
	char **new_word);
t_token	*create_merged_wildcard_token(t_token *tmp);
t_token	*handle_token_expansion(t_all *all, t_token *tmp);
t_token	*expand_tokens(t_all *all, t_token *tokens);
char	*expand_word(t_all *all, char *word, t_token *token);
char	*expand_env(t_all *all, char *word, int *i);
t_token	*create_tokens_without_space(t_token *tokens);
t_token	*create_expand_tokens(t_all *all, t_token *tokens);
bool	is_same_quote(t_token *token, t_token *token_to_compare);
t_token	*create_regular_token(t_token *tmp);
bool	is_same_type(t_token *token, t_token *token_to_compare, t_token_type type);
t_token	*manage_wildcard(t_token *to_expand);

//---------------------WILDCARD--------------------------

t_token	*expand_wildcard(t_token *tokens);
char	*merge_wildcard_values(t_token *wildcard_tokens);
bool	ft_match_pattern(char *str, char *pattern);

//---------------------INIT------------------------------

void	init_all(t_all *all, int ac, char **av, char **envp);
t_env	*fill_env(char **envp);
t_string_list	*ft_lstnew(char *content);
t_string_list	*ft_lstlast(t_string_list *lst);
void	ft_lstadd_back(t_string_list **lst, t_string_list *new);
int		ft_lstsize(t_string_list *lst);
void	ft_lstclear(t_string_list **lst, void (*del)(void *));

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