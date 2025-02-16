#include "../../includes/minishell.h"

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
	space_after = (ft_isspace(*(*str + 1)));
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
	(*token)->value = value;
}

t_token	*verif_and_edit_operator(char **str)
{
	t_token			*token;

	token = NULL;
	if (**str == ';')
		edit_token(&token, SEMICOLON, ft_strdup(";"));
	else if (**str == '~')
		edit_token(&token, TILDE, ft_strdup("~"));
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
	if (!*str)
		return (NULL);
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

bool	is_redirect(t_token_type type)
{
	return (type == REDIR_IN || type == REDIR_OUT
		|| type == REDIR_APPEND || type == REDIR_HEREDOC);
}

bool	manage_error_redir(t_all *all, t_token *token)
{
	if (is_redirect(token->token_type) 
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
		if (!ft_strcmp(token->next->value, "*") && is_redirect(token->token_type))
		{
			all->status = 1;
			return (ft_putstr_fd("ministress: *: ambiguous redirect\n", 2), true);
		}
	}
	return (false);
}

bool	is_error_expansion_char(char c)
{
	return (c == ')' || c == '(' || c == '{' || c == '-' || c == '#');
}

bool	manage_expansion_error(t_all *all, t_token *token)
{
	int	i;

	i = 0;
	if (token->token_type == WORD)
	{
		while (token->value[i])
		{
			if ((token->value[i] == '$' || token->value[i] == '~') && is_error_expansion_char(token->value[i + 1]) && !token->value[i + 2])
			{
				all->status = 2;
				printf("ministress: syntax error near unexpected token `%c\'\n", token->value[i + 1]);
				return (true);
			}
			i++;
		}
	}
	else if (token->token_type == TILDE && token->next)
	{
		if (!ft_strcmp(token->next->value, ")") || !ft_strcmp(token->next->value, "("))
		{
			all->status = 2;
			printf("ministress: syntax error near unexpected token `%s\'\n", token->next->value);
			return (true);
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

bool is_same_type(t_token *token, t_token *token_to_compare, t_token_type type)
{
	if (!token || !token_to_compare)
		return (false);
	if (token->token_type == type && token_to_compare->token_type == type)
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

t_token	*join_tokens(t_token *tmp, t_token *next, t_token **next_ptr)
{
	t_token	*new;
	char	*joined_value;

	joined_value = join_token_values(tmp, next);
	new = token_new(tmp->token_type, joined_value,
			tmp->single_quoted, tmp->double_quoted);
	new->space_after = next->space_after;
	*next_ptr = next->next;
	return (new);
}

t_token	*copy_token(t_token *tmp, t_token **next_ptr)
{
	t_token	*new;

	new = token_new(tmp->token_type, ft_strdup(tmp->value),
			tmp->single_quoted, tmp->double_quoted);
	new->space_after = tmp->space_after;
	*next_ptr = tmp->next;
	return (new);
}

t_token	*create_tokens_without_space(t_token *tokens)
{
	t_token	*new_tokens;
	t_token	*tmp;
	t_token	*next;
	t_token	*new;

	new_tokens = NULL;
	tmp = tokens;
	while (tmp)
	{
		next = tmp->next;
		if (next && is_same_type(tmp, next, WORD)
			&& is_same_quote(tmp, next) && !tmp->space_after)
			new = join_tokens(tmp, next, &tmp);
		else
			new = copy_token(tmp, &tmp);
		token_addback(&new_tokens, new);
	}
	free_all_token(tokens);
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
	if (c == '+' || c == '[' || c == ']' || c == '_' || c == '*')
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

void	skip_variable_chars(char *word, int *i, int start)
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

void	handle_dollar_sign(t_all *all, char *word, int *i,
	char **new_word)
{
	char	*expansion;

	if (word[*i + 1] == '?')
		expansion = expand_status(all, i);
	else if (ft_isdigit(word[*i + 1]))
		expansion = expand_digit(i);
	else if (ft_isalnum(word[*i + 1]) || (!ft_isspecial(word[*i + 1] || word[*i + 1] == '_')))
	{
		expansion = expand_variable(all, word, i);
		*new_word = ft_strjoin(*new_word, expansion, 1, 1);
	}
	else
	{
		if (ft_isspecial(word[*i + 1] && word[*i + 1] != '_') && word[*i + 2])
		{
			(*i) += 2;
			*new_word = ft_strjoinchar(*new_word, word[(*i)++]);
		}
		else
			*new_word = ft_strjoinchar(*new_word, word[(*i)++]);
	}
}

char	*expand_word(t_all *all, char *word, t_token *tmp)
{
	int		i;
	char	*new_word;

	i = 0;
	new_word = ft_strdup("");
	while (word[i])
	{
		if (word[i] == '$' && word[i + 1])
		{
			handle_dollar_sign(all, word, &i, &new_word);
			tmp->expand_wildcard = false;
		}
		else if (word[i] == '$' && word[i + 1] && word[i + 1] == '*')
		{
			word += i + 2;
			new_word = ft_strjoin(new_word, word, 1, 0);
			if (ft_strchr(word, '*') && (!tmp->single_quoted || !tmp->double_quoted))
				tmp->expand_wildcard = true;
			else
				tmp->expand_wildcard = false;
			break ;
		}
		else
		{
			new_word = ft_strjoinchar(new_word, word[i++]);
			tmp->expand_wildcard = true;
		}
	}
	return (new_word);
}

t_token	*handle_token_expansion(t_all *all, t_token *tmp)
{
	t_token	*new;
	char	*expanded_value;

	if (tmp->token_type == WORD && !tmp->single_quoted)
	{
		expanded_value = expand_word(all, tmp->value, tmp);
		if (!expanded_value)
			return (NULL);
		new = token_new(tmp->token_type, expanded_value,
				tmp->single_quoted, tmp->double_quoted);
		new->expand_wildcard = tmp->expand_wildcard;
	}
	else
	{
		new = token_new(tmp->token_type, ft_strdup(tmp->value),
			tmp->single_quoted, tmp->double_quoted);
		new->expand_wildcard = true;
	}
	return (new);
}

t_token	*expand_tokens(t_all *all, t_token *tokens)
{
	t_token	*tmp;
	t_token	*expanded_tokens;
	t_token	*new;

	expanded_tokens = NULL;
	tmp = tokens;
	while (tmp)
	{
		new = handle_token_expansion(all, tmp);
		if (!new)
		{
			free_all_token(expanded_tokens);
			free_all_token(tokens);
			return (NULL);
		}
		new->space_after = tmp->space_after;
		if (!*new->value)
			new->space_after = false;
		token_addback(&expanded_tokens, new);
		tmp = tmp->next;
	}
	return (expanded_tokens);
}

t_token *merge_tokens(t_token *tokens)
{
    t_token *curr;
    t_token *next;
    bool     merged;

    if (!tokens || !tokens->next)
        return (tokens);
    merged = 0;
    curr = tokens;
    while (curr && curr->next)
    {
        if (is_same_type(curr->next, curr, WORD) && !curr->space_after)
        {
            curr->value = join_token_values(curr, curr->next);
            next = curr->next;
            curr->next = curr->next->next;
			curr->space_after = next->space_after;
            free(next->value);
            free(next);
            merged = true;
            break;
        }
        curr = curr->next;
    }
    if (merged)
        return (merge_tokens(tokens));
        
    return (tokens);
}


t_token *create_expand_tokens(t_all *all, t_token *tokens)
{
	t_token *expanded_tokens;

	if (!tokens)
		return (NULL);
	expanded_tokens = create_tokens_without_space(tokens);
	expanded_tokens = expand_tokens(all, expanded_tokens);
	// print_tokens(expanded_tokens);
	// printf("\n\n\nBEFORE MERGE\n\n");
	// print_tokens(expanded_tokens);
	// expanded_tokens = merge_tokens(expanded_tokens);
	// printf("\n\n\nAFTER MERGE\n\n");
	// print_tokens(expanded_tokens);
	return (expanded_tokens);
}

char	*merge_wildcard_values(t_token *wildcard_tokens)
{
	t_token	*curr;
	char	*merged;
	char	*tmp;

	if (!wildcard_tokens)
		return (ft_strdup("*"));
	merged = ft_strdup(wildcard_tokens->value);
	if (!merged)
		return (NULL);
	curr = wildcard_tokens->next;
	while (curr)
	{
		tmp = ft_strjoin(merged, " ", 1, 0);
		if (!tmp)
			return (NULL);
		merged = ft_strjoin(tmp, curr->value, 1, 0);
		if (!merged)
			return (NULL);
		curr = curr->next;
	}
	return (merged);
}

t_token	*create_regular_token(t_token *tmp)
{
	t_token	*new;
	char	*str_dup;

	str_dup = ft_strdup(tmp->value);
	if (!str_dup)
		return (NULL);
	new = token_new(tmp->token_type, str_dup,
			tmp->single_quoted, tmp->double_quoted);
	if (!new)
		return (NULL);
	new->space_after = tmp->space_after;
	return (new);
}

t_token	*create_merged_wildcard_token(t_token *tmp)
{
	t_token	*wildcard_tokens;
	t_token	*new;
	char	*merged_value;
	bool	had_space;

	wildcard_tokens = manage_wildcard(tmp);
	if (!wildcard_tokens)
		return (create_regular_token(tmp));
	merged_value = merge_wildcard_values(wildcard_tokens);
	if (!merged_value)
	{
		free_all_token(wildcard_tokens);
		return (NULL);
	}
	had_space = tmp->space_after;
	new = token_new(tmp->token_type, merged_value,
			tmp->single_quoted, tmp->double_quoted);
	if (!new)
		return (NULL);
	new->space_after = had_space;
	free_all_token(wildcard_tokens);
	return (new);
}

bool	is_expandable_wildcard(t_token *token)
{
	if (token->token_type == WORD && ft_strchr(token->value, '*')
		&& !token->double_quoted
		&& token->expand_wildcard)
		return (true);
	return (false);
}

t_token	*expand_wildcard(t_token *tokens)
{
	t_token	*tmp;
	t_token	*expanded_tokens;
	t_token	*new;

	if (!tokens)
		return (NULL);
	tmp = tokens;
	expanded_tokens = NULL;
	print_tokens(tokens);
	while (tmp)
	{
		if (is_expandable_wildcard(tmp))
		{
			new = create_merged_wildcard_token(tmp);
			edit_token(&new, WORD, new->value);
		}
		else
			new = create_regular_token(tmp);
		if (!new)
			return (free_all_token(expanded_tokens), NULL);
		token_addback(&expanded_tokens, new);
		tmp = tmp->next;
	}
	return (expanded_tokens);
}


bool	is_hidden_valid(char *str, char *pattern)
{
	if (*str == '.' && *pattern != '.')
		return (false);
	return (true);
}

bool	ft_match_pattern(char *str, char *pattern)
{
	if (!*pattern)
		return (!*str);
	if (!is_hidden_valid(str, pattern))
		return (false);
	if (*pattern == '*' && *(pattern + 1) == '*')
		return (ft_match_pattern(str, pattern + 1));
	if (*pattern == '*')
		return (ft_match_pattern(str, pattern + 1)
			|| (*str && ft_match_pattern(str + 1, pattern)));
	if (*pattern == '?' && *str)
		return (ft_match_pattern(str + 1, pattern + 1));
	if (*pattern == *str)
		return (ft_match_pattern(str + 1, pattern + 1));
	return (false);
}

void	sort_matches(char **matches, int count)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strcmp(matches[j], matches[j + 1]) > 0)
			{
				temp = matches[j];
				matches[j] = matches[j + 1];
				matches[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

t_token	*create_tokens_from_matches(char **matches,
				t_token *to_expand)
{
	t_token	*expanded_tokens;
	t_token	*new;
	int		i;

	expanded_tokens = NULL;
	i = 0;
	while (matches[i])
	{
		new = token_new(WORD, matches[i], to_expand->single_quoted,
				to_expand->double_quoted);
		if (!new)
		{
			free_all_token(expanded_tokens);
			return (NULL);
		}
		new->expand_wildcard = false;
		token_addback(&expanded_tokens, new);
		i++;
	}
	return (expanded_tokens);
}

char	**store_matches(DIR *dir, t_token *to_expand, int *match_count)
{
	struct dirent	*entry;
	char			**matches;
	int				i;

	*match_count = 0;
	while ((entry = readdir(dir)))
		if (ft_match_pattern(entry->d_name, to_expand->value))
			(*match_count)++;
	matches = (char **)malloc(sizeof(char *) * (*match_count + 1));
	if (!matches)
		return (NULL);
	rewinddir(dir);
	i = 0;
	while ((entry = readdir(dir)) && i < *match_count)
	{
		if (ft_match_pattern(entry->d_name, to_expand->value))
		{
			matches[i] = ft_strdup(entry->d_name);
			if (!matches[i])
				return (NULL);
			i++;
		}
	}
	matches[i] = NULL;
	return (matches);
}

t_token	*manage_wildcard(t_token *to_expand)
{
	DIR		*dir;
	char	**matches;
	t_token	*expanded_tokens;
	int		match_count;

	if (!to_expand || !to_expand->value)
		return (NULL);
	dir = opendir(".");
	if (!dir)
		return (NULL);
	matches = store_matches(dir, to_expand, &match_count);
	if (!matches)
	{
		closedir(dir);
		return (NULL);
	}
	sort_matches(matches, match_count);
	expanded_tokens = create_tokens_from_matches(matches, to_expand);
	closedir(dir);
	free(matches);
	return (expanded_tokens);
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
		all->tokens = create_expand_tokens(all, all->tokens);
		all->tokens = expand_wildcard(all->tokens);
		all->tokens = merge_tokens(all->tokens);
		// print_tokens(all->tokens);
		free(line);
		free_all_token(all->tokens);
	}
}

int	main(int ac, char **av, char **envp)
{
	t_all all;
	// char	*keys[] = {NULL};

	init_all(&all, ac, av, envp);
	minishell_loop(&all);
	free_all(&all);
	return (0);
}