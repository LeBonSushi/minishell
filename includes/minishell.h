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
# define HEREDOC_TMP ".heredoc"

# include "../libft/libft.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <limits.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <termios.h>
# include <string.h>
# include <strings.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_zebi
{
	int						g_exit;
	int						g_in_cmd_execution;
	int						in_here;
	int						heredoc_interrupted;
}							t_zebi;

extern t_zebi				g_zebi;

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
}							t_token_type;

typedef enum e_node_type
{
	CMD = 1,
	OPERATOR_PIPE,
	OPERATOR_SEMICOLON,
	OPERATOR_AND,
	OPERATOR_OR,
}							t_node_type;

typedef struct s_env
{
	char					*key;
	char					*value;
	struct s_env			*next;
	int						count;
}							t_env;

typedef struct s_env_no_sorted
{
	char					*key;
	char					*value;
	struct s_env_no_sorted	*next;
}							t_env_no_sorted;

typedef struct s_token
{
	t_token_type			token_type;
	bool					single_quoted;
	bool					double_quoted;
	bool					space_after;
	bool					expanded;
	char					*value;
	struct s_token			*next;
}							t_token;

typedef struct s_cmd
{
	t_token					*tokens;
	pid_t					pid;
	bool					skip_cmd;
	bool					here_doc;
	char					*path;
	char					**args;
	int						infile_fd;
	int						outfile_fd;
	struct s_cmd			*next;
}							t_cmd;

typedef struct s_string_list
{
	char					*str;
	t_token_type			redir_type;
	struct s_string_list	*next;
}							t_string_list;

typedef struct s_all
{
	t_env					*env;
	t_env					*env_no_sorted;
	t_token					*tokens;
	char					**paths;
	pid_t					pid;
	t_cmd					*cmds;
	int						status;
	char					*limiter;
}							t_all;

void						free_all2(t_all *all);
bool						ft_atoll(const char *str, long long *result);

//---------------------ENV-------------------------------

void						print_env(t_env *env);
void						print_sorted_env(t_env *env);
void						update_env(t_env **env, char *name, char *val,
								bool overwrite);
char						*ft_getenv_value(t_env *env, char *key);
t_env						*getnode_env_value(t_env *env, char *key);
t_env						*env_new(char *key, char *value);
t_env						*env_get_last(t_env *env);
void						env_addback(t_env **env, t_env *new);
char						*find_key(const char *envp_value);
char						*find_value(const char *envp_value);
t_env						*env_create_sorted(t_env *env);

//---------------------TOKENS----------------------------

bool						is_operator(char *str);
bool						is_quote(char c);
bool						is_quotes_closed(char *str);
bool						is_valid_tokens(t_token *token);
void						delete_token(t_token **head_ref,
								t_token *node_to_delete);
void						edit_token(t_token **token, t_token_type type,
								char *value);
t_token						*token_new(t_token_type token_type, char *value,
								bool single_q, bool double_q);
void						token_addback(t_token **lst, t_token *new);
void						manage_empty_tokens(t_token **tokens);
t_token						*ft_parse_quoted(char **str, bool *sq, bool *dq);
t_token						*ft_parse_word(char **str, bool sq, bool dq);
t_token						*ft_parse_operator(char **str);
t_token						*new_parsed_token(char **str);
t_token						*fill_tokens(char *str);
t_token						*join_tokens(t_token *tmp, t_token *next,
								t_token **next_ptr);
t_token						*merge_tokens(t_token *tokens);
bool						manage_expansion_error(t_token *token);
bool						is_error_expansion_char(char c);

//---------------------EXPANDER--------------------------

char						*get_env_value(t_all *all, char *var_name);
bool						ft_isspecial(char c);
char						*ft_strjoinchar(char *s1, const char c);
char						*expand_status(t_all *all, int *i);
void						handle_variable_expansion(t_all *all, char *word,
								int *i, char **new_word);
char						*get_value(t_env *env, char *key);
t_token						*token_new4(t_token_type token_type, char *value,
								bool single_q, bool double_q);
bool						manage_error_redir(t_token *token);
char						*join_token_values(t_token *first, t_token *second);
void						skip_variable_chars(char *word, int *i);
void						handle_dollar_sign(t_all *all, char *word, int *i,
								char **new_word);
t_token						*handle_token_expansion(t_all *all, t_token *tmp);
t_token						*expand_tokens(t_all *all, t_token *tokens);
char						*expand_word(t_all *all, char *word);
t_token						*create_expand_tokens(t_all *all, t_token *tokens);
bool						is_same_quote(t_token *token,
								t_token *token_to_compare);
bool						is_same_type(t_token *token,
								t_token *token_to_compare, t_token_type type);

//---------------------INIT------------------------------

void						init_all(t_all *all, int ac, char **av,
								char **envp);
t_env						*fill_env(char **envp);
void						init_readline(void);

//---------------------DESTROY---------------------------

void						free_env(t_env *env);
void						free_all_env(t_env *env);
void						free_all_token(t_token *token);
void						free_all(t_all *all);
void						free_all_cmds(t_cmd *cmds);
void						skip_all_cmds(t_cmd *cmds);
void						free_multiple(t_all *all, char **env);

//--------------------BUILTINS---------------------------

bool						is_builtin(char *cmd);
void						execute_builtins(t_cmd *cmd, char **args,
								t_env **env, t_all *all);
void						ft_unset(t_token *tokens, t_env **env);
void						ft_echo(t_token *tokens);
void						ft_cd(t_token *tokens, t_env *env);
void						ft_pwd(void);
void						ft_env(t_env *env);
void						ft_exit(t_token *tokens, t_all *all);
void						unset_value(t_env **env_head, t_env *env_to_unset);
void						ft_export(t_token *tokens, t_env **env);
bool						check_valid_key_export(const char *key);
bool						find_egal_in_token(char *str);

//---------------------PARSING---------------------------

void						minishell_loop(t_all *all);

//------------------------CMDS-------------------------

t_cmd						*create_cmdlist(t_token *token);
t_cmd						*cmd_new(t_token *tokens);
void						cmd_addback(t_cmd **cmd, t_cmd *new);
int							manage_redirections(t_all *all, t_cmd *cmds);

//---------------------UTILS---------------------------

bool						is_redir(t_token *token);
void						delete_token2(t_token **tokens,
								t_token *token_to_del);
char						**env_to_tab(t_env *env);
void						free_tab(char **tab);
bool						ft_compare(char *limiter, char *line);

//---------------------REDIRECTION---------------------------

char						*get_cmd_path(t_all *all, char *cmd, char **env);
char						**tokens_to_tab(t_token *tokens);
void						exec_cmds(t_all *all, t_cmd *cmds);
int							ft_exit2(t_all *all, int exit_code);
t_token						*copy_token(t_token *src);
bool						contain_redir_errors(t_cmd *cmds, int *status,
								bool print);

//---------------------HEREDOC-----------------------------

int							do_heredoc(t_all *all, t_cmd *cmd,
								t_token *curr_token, bool is_last);
bool						is_last(t_token *token);
int							redir_in(t_cmd *curr_cmd,
								t_token *curr_token);
bool						is_redir_out(t_token *token);
int							manage_cmd_redir(t_all *all, t_cmd *curr_cmd);

//----------------------EXEC-------------------------------

bool						is_dir(t_all *all, char *cmd);
char						*find_exec_path(char *cmd, char **paths, char **args);
char						**get_paths(t_all *all, char **env);
int							tokens_size(t_token *tokens);
bool						is_file(char *cmd);
void						exec_error(char *cmd);
char						*get_cmd_path(t_all *all, char *cmd, char **env);
void						pipe_error(t_all *all, int *fd);
void						fork_error(t_all *all, int *fd);
void						do_pipes(t_all *all, t_cmd *curr, char **env,
								int fd[2]);
void						waitall(t_cmd *cmds);
void						close_fds(t_cmd *cmd);

//---------------------SIGNAL-------------------------------

void						handle_sigint_here(int sig);
void						handle_sigint(int sig);
void						handle_sigquit(int sig);
void						setup_signals(void);
void						set_execution_mode(void);
void						set_interactive_mode(void);

#endif
