/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 04:28:23 by macorso           #+#    #+#             */
/*   Updated: 2025/04/29 18:19:57 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

t_token	*token_new2(t_token_type token_type, char *value,
			bool single_q, bool double_q)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->token_type = token_type;
	new->value = value;
	new->single_quoted = single_q;
	new->double_quoted = double_q;
	new->space_after = true;
	new->next = NULL;
	return (new);
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
			new = join_tokens(tmp, next, &next);
		else
		{
			new = token_new(tmp->token_type, tmp->value, tmp->single_quoted, tmp->double_quoted);
			tmp = next;
		}
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
	if (c == '+' || c == '[' || c == ']' || c == '_' || c == '*' || c == '#' || c == '@')
		return (true);
	return (false);
}

// char	*ft_strjoinchar(char *s1, const char c)
// {
// 	int	len;
// 	char	*str;
// 	len = ft_strlen(s1);
	
// 	str = (char *)malloc(sizeof(char) * (len + 2));
// 	if (!str)
// 		return (NULL);
// 	ft_strlcpy(str, s1, len + 1);
// 	str[len] = c;
// 	str[len + 1] = '\0';
// 	free(s1);
// 	return (str);
// }

char	*ft_strjoinchar(char *s1, const char c)
{
	int		len;
	char	*str;

	if (!s1)
	{
		str = malloc(2);
		if (!str)
			return (NULL);
		str[0] = c;
		str[1] = '\0';
		return (str);
	}
	len = ft_strlen(s1);
	str = (char *)malloc(sizeof(char) * (len + 2));
	if (!str)
	{
		free(s1);
		return (NULL);
	}
	ft_strlcpy(str, s1, len + 1);
	str[len] = c;
	str[len + 1] = '\0';
	free(s1);
	return (str);
}

void	skip_variable_chars(char *word, int *i)
{
	while (word[*i] && (ft_isalnum(word[*i]) || word[*i] == '_'))
	{
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
	skip_variable_chars(word, i);
	var_name = ft_substr(word, start, *i - start);
	// printf("%s\n", var_name);
	value = get_env_value(all, var_name);
	free(var_name);
	return (value);
}

void	handle_variable_expansion(t_all *all, char *word, int *i, char **new_word)
{
	char	*expansion;
	
	// printf("ouais\n");
	expansion = expand_variable(all, word, i);
	*new_word = ft_strjoin(*new_word, expansion, 1, 1);
}

void	handle_special_char(char *word, int *i, char **new_word)
{
	if (ft_isspecial(word[*i + 1]) && word[*i + 1] != '_' && word[*i + 2])
	{
		(*i) += 2;
		*new_word = ft_strjoinchar(*new_word, word[(*i)++]);
	}
	else
		*new_word = ft_strjoinchar(*new_word, word[(*i)++]);
}

void	handle_dollar_sign(t_all *all, char *word, int *i, char **new_word)
{
	// printf("Ouais\n");
	if (word[*i + 1] == '?')
		*new_word = ft_strjoin(*new_word, expand_status(all, i), 1, 1);
    else if (!ft_isalpha(word[*i + 1]) && !ft_isdigit(word[*i + 1]) && word[*i + 1] != '_')
		*new_word = ft_strjoinchar(*new_word, word[(*i)++]);
    else
        handle_variable_expansion(all, word, i, new_word);
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

char	*get_value(t_env *env, char *key)
{
	while (env)
	{
		if (!ft_strcmp(env->key, key))
			return (ft_strdup(env->value));
		env = env->next;
	}
	return (ft_strdup(""));
}

t_token	*token_new4(t_token_type token_type, char *value,
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
	new->space_after = true;
	free(value);
	new->next = NULL;
	return (new);
}

t_token	*handle_token_expansion(t_all *all, t_token *tmp)
{
	t_token	*new;
	char	*expanded_value;

	if (tmp->token_type == WORD && !tmp->single_quoted)
	{
		expanded_value = expand_word(all, tmp->value);
		if (!expanded_value)
			return (NULL);
		new = token_new4(tmp->token_type, expanded_value,
				tmp->single_quoted, tmp->double_quoted);
	}
	else if (tmp->token_type == TILDE && !tmp->single_quoted)
	{
		expanded_value = get_value(all->env, "HOME");
		if (!expanded_value)
			return (NULL);
		new = token_new(WORD, expanded_value,
				tmp->single_quoted, tmp->double_quoted);
	}
	else
	{
		new = token_new4(tmp->token_type, ft_strdup(tmp->value),
			tmp->single_quoted, tmp->double_quoted);
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
		if (!*new->value && ft_strcmp(tmp->value, ""))
			new->space_after = false;
		token_addback(&expanded_tokens, new);
		tmp = tmp->next;
	}
	return (expanded_tokens);
}

bool	is_redir(t_token *token)
{
	return (token->token_type == REDIR_IN
		|| token->token_type == REDIR_OUT
		|| token->token_type == REDIR_APPEND
		|| token->token_type == REDIR_HEREDOC);
}

void	free_token(t_token *token)
{
	if (token)
		return (free(token->value), free(token));
}

static void init_tokens(bool *merged, t_token **curr, t_token *tokens)
{
	*merged = false;
	*curr = tokens;
}

t_token *merge_tokens(t_token *tokens)
{
	t_token *curr;
	t_token *next;
	bool     merged;

	if (!tokens || !tokens->next)
		return (tokens);
	init_tokens(&merged, &curr, tokens);
	while (curr && curr->next)
	{
		if (!is_redir(curr) && !is_redir(curr->next) && curr->token_type != PIPE
			&& curr->next->token_type != PIPE && !curr->space_after)
		{
			curr->value = join_token_values(curr, curr->next);
			next = curr->next;
			curr->next = curr->next->next;
			curr->space_after = next->space_after;
			free_token(next);
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
	t_token *token_after;

	if (!tokens)
		return (NULL);
	token_after = create_tokens_without_space(tokens);
	expanded_tokens = expand_tokens(all, token_after);
	free_all_token(token_after);
	return (expanded_tokens);
}