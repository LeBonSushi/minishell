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
# define HEREDOC_TMP ".heredoctmp"

# include "../libft/libft.h"

# include <stdio.h>
# include <dirent.h>
# include <stdlib.h>
# include <stdbool.h>
# include <signal.h>
# include <fcntl.h>
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
	OPEN_PARENTHESIS,
	CLOSE_PARENTHESIS,
	TILDE,
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
	int				count;
}	t_env;

typedef struct s_env_no_sorted
{
	char					*key; // name before the = sign
	char					*value; // name after the = sign
	struct s_env_no_sorted	*next;
}	t_env_no_sorted;

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

typedef struct s_cmd
{
	t_token	*tokens;
	pid_t	pid;
	bool	skip_cmd;
	bool	here_doc;
	char	*path;
	int		infile_fd;
	int		outfile_fd;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_string_list
{
	char					*str;
	t_token_type			redir_type;
	struct s_string_list	*next;
}	t_string_list;

typedef struct s_all
{
	t_env	*env;
	t_env	*env_no_sorted;
	t_token	*tokens;
	pid_t	pid;
	t_string_list	*redir_in_names;
	t_string_list	*redir_out_names;
	t_cmd			*cmds;
	int		status;
}	t_all;

void	print_header(void);
void	print_tab(char **tab);

//---------------------ENV-------------------------------

void		print_env(t_env *env);
void		update_env(t_env **env, char *name, char *val, bool overwrite);
void		ft_stradd_back(t_string_list **lst, t_string_list *new);
void		free_str_list(t_string_list *lst);
char			*ft_getenv_value(t_env *env, char *key);
t_string_list	*ft_strnew(char *content, t_token_type redir_type);
t_string_list	*ft_strlast(t_string_list *lst);

t_env	*env_new(char *key, char *value);
t_env	*env_get_last(t_env	*env);
void	env_addback(t_env **env, t_env *new);
char	*find_key(const char *envp_value);
char	*find_value(const char *envp_value);
t_env	*env_create_sorted(t_env *env);

//---------------------TOKENS----------------------------

bool			is_operator(char *str);
bool			is_quote(char c);
bool			is_quotes_closed(char *str);
bool			is_valid_tokens(t_all *all, t_token *token);
void			delete_token(t_token **tokens, t_token *token_to_del);
void			edit_token(t_token **token, t_token_type type, char *value);
t_token			*token_new(t_token_type token_type, char *value,
			bool single_q, bool double_q);
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
// bool	is_expandable_wildcard(t_token *token);
// bool	is_hidden_valid(char *str, char *pattern);
// void	sort_matches(char **matches, int count);
// t_token	*create_tokens_from_matches(char **matches,
			// t_token *to_expand);
// char	**store_matches(DIR *dir, t_token *to_expand, int *match_count);
void	skip_variable_chars(char *word, int *i, int start);
void	handle_dollar_sign(t_all *all, char *word, int *i,
		char **new_word);
// t_token	*create_merged_wildcard_token(t_token *tmp);
t_token	*handle_token_expansion(t_all *all, t_token *tmp);
t_token	*expand_tokens(t_all *all, t_token *tokens);
char	*expand_word(t_all *all, char *word);
t_token	*create_tokens_without_space(t_token *tokens);
t_token	*create_expand_tokens(t_all *all, t_token *tokens);
bool	is_same_quote(t_token *token, t_token *token_to_compare);
// t_token	*create_regular_token(t_token *tmp);
bool	is_same_type(t_token *token, t_token *token_to_compare, t_token_type type);
// t_token	*manage_wildcard(t_token *to_expand);

//---------------------WILDCARD--------------------------

// t_token	*expand_wildcard(t_token *tokens);
// char	*merge_wildcard_values(t_token *wildcard_tokens);
// bool	ft_match_pattern(char *str, char *pattern);

//---------------------INIT------------------------------

void	init_all(t_all *all, int ac, char **av, char **envp);
t_env	*fill_env(char **envp);
t_string_list	*ft_lstnew(char *content);
t_string_list	*ft_lstlast(t_string_list *lst);
void	ft_lstadd_back(t_string_list **lst, t_string_list *new);
int		ft_lstsize(t_string_list *lst);
void	ft_strclear(t_string_list **lst);

//---------------------DESTROY---------------------------

void	free_env(t_env *env);
void	free_all_env(t_env *env);
void	free_all_token(t_token *token);
void	free_all(t_all *all);

//--------------------BUILTINS---------------------------


bool	is_builtin(t_token *tokens);
void	execute_builtins(t_token *tokens, t_env **env, t_all *all);
void	ft_unset(t_token *tokens, t_env **env);
void	ft_echo(t_token *tokens);
void	ft_cd(t_token *tokens, t_env *env);
void	ft_pwd(void);
void	ft_env(t_env *env);
void	ft_exit(t_token *tokens, t_all *all);

//---------------------EXPORT----------------------------

void	ft_export(t_token *tokens, t_env **env);
bool	check_valid_key_export(const char *key);
bool	find_egal_in_token(char *str);

//---------------------PARSING---------------------------

void	minishell_loop(t_all *all, char **envp);

//------------------------CMDS-------------------------

t_cmd	*create_cmdlist(t_token *token);
t_cmd	*cmd_new(t_token	*tokens, char *path);
void	cmd_addback(t_cmd **cmd, t_cmd *new);
void	manage_redirections(t_all *all, t_cmd *cmds);

//---------------------CLEAR---------------------------

void	clear_terminal(void);

//---------------------UTILS---------------------------

bool	is_redir(t_token *token);
void	print_list(t_string_list *list);
void	delete_token2(t_token **tokens, t_token *token_to_del);
void	debug_print_cmdlist(t_cmd *cmd_list);
char **env_to_tab(t_env *env);
void	free_tab(char **tab);

//---------------------REDIRECTION---------------------------

void	ft_redirection(t_all *all, t_token *token);
// void	ft_redirection(t_token *token, t_env *env, char **envp);

#endif
