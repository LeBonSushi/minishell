/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 13:30:19 by macorso           #+#    #+#             */
/*   Updated: 2025/02/12 22:56:10 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool is_same_type(t_token *token, t_token *token_to_compare)
{
	if (!token || !token_to_compare)
		return (false);
	if (token->token_type == token_to_compare->token_type)
		return (true);
	return (false);
}

bool is_same_quote(t_token *token, t_token *token_to_compare)
{
	if (!token || !token_to_compare)
		return (false);
	if (token->single_quoted == token_to_compare->single_quoted
		&& token->double_quoted == token_to_compare->double_quoted)
		return (true);
	return (false);
}

char *join_token_values(t_token *first, t_token *second)
{
	char *joined;
	char *temp;

	temp = ft_strdup(first->value);
	joined = ft_strjoin(temp, second->value, 1, 0);
	return (joined);
}

t_token *create_tokens_without_space(t_token *tokens)
{
	t_token *new_tokens;
	t_token *tmp;
	t_token *next;
	t_token *new;
	char *joined_value;

	new_tokens = NULL;
	tmp = tokens;
	while (tmp)
	{
		next = tmp->next;
		if (next && is_same_type(tmp, next) && is_same_quote(tmp, next) && !tmp->space_after)
		{
			joined_value = join_token_values(tmp, next);
			new = token_new(tmp->token_type, joined_value, tmp->single_quoted, tmp->double_quoted);
			tmp = next->next;  // Skip the next token since we've joined it
		}
		else
		{
			new = token_new(tmp->token_type, ft_strdup(tmp->value), tmp->single_quoted, tmp->double_quoted);
			tmp = next;
		}
		new->space_after = tmp->space_after;
		token_addback(&new_tokens, new);
	}
	return (new_tokens);
}

char *get_env_value(t_all *all, char *var_name)
{
	t_env *env;

	env = all->env;
	while (env)
	{
		if (!ft_strcmp(env->key, var_name))
			return (ft_strdup(env->value));
		env = env->next;
	}
	return (ft_strdup(""));  // Return empty string if not found
}

char *expand_status(t_all *all, int *i)
{
	(*i) += 2;  // Skip $?
	return (ft_itoa(all->status));
}

char *expand_digit(int *i)
{
	(*i) += 2;  // Skip $n
	return (ft_strdup(""));  // $n expands to nothing in bash when n is a digit
}

bool	ft_isspecial(char c)
{
	if (c == '+' || c == '[' || c == ']' || c == '*')
		return (true);
	return (false);
}

char	*ft_strjoinchar(const char *s1, const char c)
{
	int	len;
	char	*str;
	len = ft_strlen(s1);
	
	str = (char *)malloc(sizeof(char) * (len + 2));
	if (!str)
		return (NULL);
	ft_strlcpy(str, s1, len + 1);
	str[len] = c;
	str[len + 1] = '\0';
	free((char *)s1);
	return (str);
}

static void	skip_variable_chars(char *word, int *i, int start)
{
	while (word[*i] && (ft_isalnum(word[*i]) || ft_isspecial(word[*i])))
	{
		if (word[*i] == '*' && *i > start)
			break ;
		(*i)++;
	}
}

char	*expand_variable(t_all *all, char *word, int *i)
{
	int		start;
	char	*var_name;
	char	*value;

	(*i)++;
	start = *i;
	skip_variable_chars(word, i, start);
	var_name = ft_substr(word, start, *i - start);
	value = get_env_value(all, var_name);
	free(var_name);
	return (value);
}

static void	handle_dollar_sign(t_all *all, char *word, int *i,
	char **new_word)
{
	char	*expansion;

	if (word[*i + 1] == '?')
		expansion = expand_status(all, i);
	else if (ft_isdigit(word[*i + 1]))
		expansion = expand_digit(i);
	else if (ft_isalnum(word[*i + 1]) || !ft_isspecial(word[*i + 1]))
	{
		expansion = expand_variable(all, word, i);
		*new_word = ft_strjoin(*new_word, expansion, 1, 1);
	}
	else
	{
		if (ft_isspecial(word[*i + 1]) && word[*i + 2])
		{
			(*i) += 2;
			*new_word = ft_strjoinchar(*new_word, word[(*i)++]);
		}
		else
			*new_word = ft_strjoinchar(*new_word, word[(*i)++]);
	}
}

char	*expand_word(t_all *all, char *word)
{
	int		i;
	char	*new_word;

	i = 0;
	new_word = ft_strdup("");
	while (word[i])
	{
		if (word[i] == '$' && word[i + 1])
			handle_dollar_sign(all, word, &i, &new_word);
		else
			new_word = ft_strjoinchar(new_word, word[i++]);
	}
	return (new_word);
}

t_token	*expand_tokens(t_all *all, t_token *tokens)
{
	t_token *tmp;
	t_token *expanded_tokens;
	t_token *new;

	if (!tokens)
		return (NULL);
	tmp = tokens;
	expanded_tokens = NULL;
	while (tmp)
	{
		if (tmp->token_type == WORD && !tmp->single_quoted)
		{
			new = token_new(tmp->token_type, expand_word(all, tmp->value), tmp->single_quoted, tmp->double_quoted);
		}
		else
			new = token_new(tmp->token_type, ft_strdup(tmp->value), tmp->single_quoted, tmp->double_quoted);
		new->space_after = tmp->space_after;	
		tmp = tmp->next;
		token_addback(&expanded_tokens, new);
	}
	free_all_token(tokens);
	return (expanded_tokens);
}

// t_token	*manage_wildcard(t_token *token)
// {
	
// }

// t_token	*expand_wildcard(t_all *all, t_token *tokens)
// {
// 	t_token *tmp;
// 	t_token *expanded_tokens;
// 	t_token *new;

// 	if (!tokens)
// 		return (NULL);
// 	tmp = tokens;
// 	expanded_tokens = NULL;
// 	while (tmp)
// 	{
// 		if (tmp->token_type == WORD && ft_strchr(tmp->value, '*') && (!tmp->single_quoted || !tmp->double_quoted))
// 			new = manage_wildcard(tmp);
// 		else
// 			new = token_new(tmp->token_type, ft_strdup(tmp->value), tmp->single_quoted, tmp->double_quoted);
// 		new->space_after = tmp->space_after;
// 		token_addback(&expanded_tokens, new);
// 		tmp = tmp->next;
// 	}
// 	free_all_token(tokens);
// 	return (expanded_tokens);
// }

void create_expand_tokens(t_all *all, t_token *tokens)
{
	t_token *tmp;
	t_token *expanded_tokens;

	tmp = create_tokens_without_space(tokens);
	free_all_token(tokens);
	expanded_tokens = expand_tokens(all, tmp);
	tmp = create_tokens_without_space(expanded_tokens);
	// free_all_token(expanded_tokens);
	// expanded_tokens = expand_wildcard(all, tmp);
	
	all->tokens = expanded_tokens;
}

t_token	*ft_parse_quoted(char **str, bool *sq, bool *dq)
{
	char	quote;
	char	*start;
	int		len;
	t_token	*token;
	bool	space_after;

	quote = **str;
	(*str)++;
	start = *str;
	len = 0;
	*sq = (quote == '\'');
	*dq = (quote == '\"');
	while (**str && **str != quote)
	{
		(*str)++;
		len++;
	}
	if (**str != quote)
		return (NULL);
	space_after = (ft_isspace(*(*str + 1))); // || !*(*str + 1) ???
	token = token_new(WORD, ft_substr(start, 0, len),
			*sq, *dq);
	token->space_after = space_after;
	(*str)++;
	return (token);
}

void	edit_token(t_token **token, t_token_type type, char *value)
{
	if (!*token)
		*token = token_new(WORD, NULL, false, false);
	(*token)->token_type = type;
	if ((*token)->value)
		free((*token)->value);
	(*token)->value = value;
}

t_token	*verif_and_edit_operator(char **str)
{
	t_token			*token;

	token = NULL;
	if (**str == ';')
		edit_token(&token, SEMICOLON, ft_strdup(";"));
	else if (**str == '|')
	{
		if (*(*str + 1) == '|' && (*str)++)
			edit_token(&token, OR, ft_strdup("||"));
		else
			edit_token(&token, PIPE, ft_strdup("|"));
	}
	else if (**str == '&')
	{
		if (*(*str + 1) == '&' && (*str)++)
			edit_token(&token, AND, ft_strdup("&&"));
	}
	else if (**str == '<')
	{
		if (*(*str + 1) == '<' && (*str)++)
			edit_token(&token, REDIR_HEREDOC, ft_strdup("<<"));
		else
			edit_token(&token, REDIR_IN, ft_strdup("<"));
	}
	else if (**str == '>')
	{
		if (*(*str + 1) == '>' && (*str)++)
			edit_token(&token, REDIR_APPEND, ft_strdup(">>"));
		else
			edit_token(&token, REDIR_OUT, ft_strdup(">"));
	}
	else
		return (NULL);
	return (token);
}

t_token	*ft_parse_operator(char **str)
{
	t_token	*token;

	token = verif_and_edit_operator(str);
	(*str)++;
	return (token);
}


// t_token	*ft_parse_operator(char **str)
// {
// 	t_token	*token;

// 	token = token_new(WORD, NULL, false, false);
// 	if (**str == ';')
// 		edit_token(&token, SEMICOLON, ft_strdup(";"));
// 	else if (**str == '|')
// 		edit_token(&token, PIPE, ft_strdup("|"));
// 	else if (**str == '<')
// 	{
// 		if (*(*str + 1) == '<' && (*str)++)
// 			edit_token(&token, REDIR_HEREDOC, ft_strdup("<<"));
// 		else
// 			edit_token(&token, REDIR_IN, ft_strdup("<"));
// 	}
// 	else if (**str == '>')
// 	{
// 		if (*(*str + 1) == '>' && (*str)++)
// 			edit_token(&token, REDIR_APPEND, ft_strdup(">>"));
// 		else
// 			edit_token(&token, REDIR_OUT, ft_strdup(">"));
// 	}
// 	else
// 		return (NULL);
// 	(*str)++;
// 	return (token);
// }

t_token	*ft_parse_word(char **str, bool sq, bool dq)
{
	char	*start;
	int		len;
	bool	space_after;
	t_token	*token;

	start = *str;
	len = 0;
	while (**str && !ft_isspace(**str) && !is_operator(*str)
		&& (!is_quote(**str) || sq || dq))
	{
		(*str)++;
		len++;
	}
	space_after = (ft_isspace(**str));
	token = token_new(WORD, ft_substr(start, 0, len), false, false);
	token->space_after = space_after;
	return (token);
}

t_token	*new_parsed_token(char	**str)
{
	t_token	*new_token;
	bool	sq;
	bool	dq;

	sq = false;
	dq = false;
	if (!*str)
		return (NULL);
	while (ft_isspace(**str))
		(*str)++;
	if (!**str)
		return (NULL);
	new_token = NULL;
	if (is_operator(*str))
		new_token = ft_parse_operator(str);
	else if (is_quote(**str) && !sq && !dq)
		new_token = ft_parse_quoted(str, &sq, &dq);
	else
		new_token = ft_parse_word(str, sq, dq);
	if (!new_token)
		return (NULL);
	return (new_token);
}

bool	is_quotes_closed(char *str)
{
	bool	sq;
	bool	dq;

	sq = false;
	dq = false;
	while (*str)
	{
		if (*str == '\'' && !dq)
			sq = !sq;
		if (*str == '\"' && !sq)
			dq = !dq;
		str++;
	}
	return (!sq && !dq);
}

bool	manage_error_redir(t_all *all, t_token *token)
{
	if (token->token_type == REDIR_IN || token->token_type == REDIR_OUT
			|| token->token_type == REDIR_APPEND
			|| token->token_type == AND || token->token_type == OR)
	{
		if (!token->next)
		{
			all->status = 2;
			ft_putstr_fd("ministress: syntax error near unexpected token `newline\'\n", 2);
			return (true);
		}
		if (token->next->token_type != WORD && token->next->token_type != WILD_CARD)
		{
			all->status = 2;
			printf("ministress: syntax error near unexpected token `%s\'\n", token->next->value);
			return (true);
		}
		if (token->next->token_type == WILD_CARD && token->token_type != AND && token->token_type != OR)
		{
			all->status = 1;
			return (ft_putstr_fd("ministress: *: ambiguous redirect\n", 2), true);
		}
	}
	return (false);
}

bool	is_error_expansion_char(char c)
{
	return (c == ')' || c == '(' || c == '{' || c == '-');
}

bool	manage_expansion_error(t_all *all, t_token *token)
{
	int	i;

	i = 0;
	if (token->token_type == WORD)
	{
		while (token->value[i])
		{
			if (token->value[i] == '$' && is_error_expansion_char(token->value[i + 1]) && !token->value[i + 2])
			{
				all->status = 2;
				printf("ministress: syntax error near unexpected token `%c\'\n", token->value[i + 1]);
				return (true);
			}
			i++;
		}
	}
	return (false);
}

bool	is_valid_tokens(t_all *all, t_token *token)
{
	if (!token)
		return (true);
	while (token)
	{
		if (manage_error_redir(all, token))
			return (false);
		if (manage_expansion_error(all, token))
			return (false);
		token = token->next;
	}
	return (true);
}

void	delete_token(t_token **tokens, t_token *token_to_del)
{
	t_token	*tmp;
	t_token	*prev;

	tmp = *tokens;
	prev = NULL;
	while (tmp)
	{
		if (tmp == token_to_del)
		{
			if (prev)
				prev->next = tmp->next;
			else
				*tokens = tmp->next;
			free(tmp->value);
			free(tmp);
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

void manage_empty_tokens(t_token **tokens)
{
	t_token *tmp;
	t_token *next;

	if (!tokens || !*tokens)
		return ;
	tmp = *tokens;
	while (tmp)
	{
		next = tmp->next;

		if (tmp->token_type == WORD && !ft_strcmp(tmp->value, ""))
		{
			if (!tmp->space_after)
				delete_token(tokens, tmp);
		}

		tmp = next;
	}
	tmp = *tokens;
	if (!tmp || !tmp->next)
		return ;
	while (tmp->next)
		tmp = tmp->next;
	tmp->space_after = false;
}

t_token	*fill_tokens(char *str, t_all *all)
{
	t_token	*tokens;

	if (!str)
		return (NULL);
	if (!is_quotes_closed(str))
		return (ft_putstr_fd("Quotes are not properly closed !\n", 2),NULL);
	tokens = NULL;
	while (*str)
		token_addback(&tokens, new_parsed_token(&str));
	manage_empty_tokens(&tokens);
	if (!is_valid_tokens(all, tokens))
		return (free_all_token(tokens), NULL);
	return (tokens);
}

t_env	*env_new(char *key, char *value)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = ft_strdup(key);
	new->value = ft_strdup(value);
	new->next = NULL;
	return (new);
}

t_env	*env_get_last(t_env	*env)
{
	t_env	*last;

	if (!env)
		return (NULL);
	last = env;
	while (last->next)
		last = last->next;
	return (last);
}

void	env_addback(t_env **env, t_env *new)
{
	t_env	*tmp;

	if (!*env)
		*env = new;
	else
	{
		tmp = env_get_last(*env);
		tmp->next = new;
	}
}

bool check_valid_key(const char *str)
{
	int i;

	i = 0;
	if (str == NULL)
		return (false);
	if (!('a' <= str[i] && str[i] <= 'z') && !('A' <= str[i] && str[i] <= 'Z') && str[i] != '_')
		return (false);

	return (true);
}

char	*find_key(const char *envp_value)
{
	int		end;
	char	*key;

	end = 0;
	if (!envp_value)
		return (NULL);
	while(envp_value[end] != '=' && envp_value[end])
		end++;
	key = ft_substr(envp_value, 0, end);
	if (!key)
		return (ft_putstr_fd("Malloc failed\n", 2), NULL);
	if (!check_valid_key(key))
	{
		free(key);
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd((char *)envp_value, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (NULL);
	}
	return (key);
}

char	*find_value(const char *envp_value)
{
	int		start;
	int		end;
	char	*value;
	
	if (!envp_value)
		return (NULL);
	end = ft_strlen(envp_value);
	if (!end)
		return (NULL);
	start = 0;
	while (envp_value[start] && envp_value[start] != '=')
		start++;
	if (envp_value[start] != '=')
		return (NULL);
	if (!envp_value[start + 1])
		return (NULL);
	value = ft_substr(envp_value, start + 1, end);
	if (!value)
		return (NULL);
	return (value);
}

char	*ft_getenv(t_all *all, char *key)
{
	t_env	*env;
	
	env = all->env;
	while (env)
	{
		if (!ft_strcmp(env->key, key))
			return (ft_strdup(env->value));
		env = env->next;
	}
	return (NULL);
}

// a function that return a t_env* linked list
t_env	*fill_env(char **envp)
{
	t_env	*env;
	int		i;
	char	*key;
	char	*value;

	i = 0;
	env = NULL;
	while (envp[i])
	{
		key = find_key(envp[i]);
		value = find_value(envp[i]);
		env_addback(&env, env_new(key, value));
		free(key);
		free(value);
		i++;
	}
	return (env);
}


t_token	*token_new(t_token_type token_type, char *value,
			bool single_q, bool double_q)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->token_type = token_type;
	new->value = ft_strdup(value);
	new->single_quoted = single_q;
	new->double_quoted = double_q;
	new->next = NULL;
	return (new);
}

void	token_addback(t_token **lst, t_token *new)
{
	t_token	*tmp;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	tmp = *lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}


void	free_env(t_env *env)
{
	free(env->key);
	free(env->value);
	free(env);
}

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	if (!tab && !*tab)
		return ;
	while (tab[i])
		free(tab[i++]);
	free(tab);
}

void	free_all_env(t_env *env)
{
	t_env	*next;

	while (env)
	{
		next = env->next;
		free_env(env);
		env = next;
	}
}

void	free_all_token(t_token *token)
{
	t_token	*next;

	if (!token)
		return ;
	while (token)
	{
		next = token->next;
		free(token);
		token = next;
	}
}

void	free_all(t_all *all)
{
	free_all_env(all->env);
	free_all_token(all->tokens);
}

void	print_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		printf("%s\n", tab[i]);
		i++;
	}
}

void	print_tokens(t_token *tokens)
{
	t_token	*tmp;
	tmp = tokens;
	while (tmp)
	{
		if (tmp->token_type == WORD)
			printf("WORD\n(%s)\n", tmp->value);
		else if (tmp->token_type == SEMICOLON)
			printf("SEMICOLON\n%s\n", tmp->value);
		else if (tmp->token_type == PIPE)
			printf("PIPE\n%s\n", tmp->value);
		else if (tmp->token_type == REDIR_IN)
			printf("REDIR_IN\n%s\n", tmp->value);
		else if (tmp->token_type == REDIR_OUT)
			printf("REDIR_OUT\n%s\n", tmp->value);
		else if (tmp->token_type == REDIR_APPEND)
			printf("REDIR_APPEND\n%s\n", tmp->value);
		else if (tmp->token_type == REDIR_HEREDOC)
			printf("REDIR_HEREDOC\n%s\n", tmp->value);
		else if (tmp->token_type == WILD_CARD)
			printf("WILD_CARD\n%s\n", tmp->value);
		else if (tmp->token_type == OR)
			printf("OR\n%s\n", tmp->value);
		else if (tmp->token_type == AND)
			printf("AND\n%s\n", tmp->value);
		else
			printf("ERROR\n%s\n", tmp->value);
		printf("single quoted: %d\n", tmp->single_quoted);
		printf("double quoted: %d\n\n", tmp->double_quoted);
		printf("space after: %s\n\n", tmp->space_after ? "true" : "false");
		tmp = tmp->next;
	}
}
 
bool	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}
bool	is_operator(char *str)
{
	if (*str == ';' || *str == '|' || *str == '<' || *str == '>')
		return (true);
	if (*str == '&' && *(str + 1) == '&')
		return (true);
	return (false);
}

void	init_all(t_all *all, int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;

	all->env = fill_env(envp);
	all->status = 0;
	all->tokens = NULL;
	all->tree = NULL;
}

bool	env_empty(t_all *all)
{
	t_env	*env;

	env = all->env;
	if (!env)
		return (true);
	return (false);
}

void	verify_env(t_all *all)
{
	char	*pwd = pwd_builtin_char();
	char	*pwd_key = ft_strjoin("PWD=", pwd, 0, 1);
	char	*keys[] = {"OLDPWD=", pwd_key,
		"PROMPT_COMMAND=RETRN_VAL=$?;logger -p local6.debug\
		\"$(history 1 | sed \"s/^[ ]*[0-9]\\+[ ]*//\" )\
		[$RETRN_VAL]\"", "SHLVL=1", NULL};
	if (env_empty(all))
		export_builtin(all, keys);
}

void	minishell_loop(t_all *all)
{
	char *line;

	while(1)
	{
		line = readline("ministress$ ");
		if (!line || !*line)
			continue ;
		add_history(line);
		all->tokens = fill_tokens(line, all);
		all->tokens = expand_tokens(all, all->tokens);
		print_tokens(all->tokens);
		free(line);
		free_all_token(all->tokens);
	}
}

int	main(int ac, char **av, char **envp)
{
	t_all all;
	// char	*keys[] = {NULL};

	init_all(&all, ac, av, envp);
	// verify_env(&all);
	// printf("\n\n\n\n");
	// export_builtin(&all, NULL);
	// print_header();
	// env_builtin(&all);
	// env_builtin(&all);
	minishell_loop(&all);
	free_all(&all);
	return (0);
}