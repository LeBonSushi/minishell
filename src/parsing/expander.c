/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 04:28:23 by macorso           #+#    #+#             */
/*   Updated: 2025/02/10 11:48:24 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
	if (c == '+' || c == '[' || c == ']' || c == '_' || c == '*')
		return (true);
	return (false);
}

char *expand_variable(t_all *all, char *word, int *i)
{
	int start;
	char *var_name;
	char *value;

	(*i)++;  // Skip $
	start = *i;
	while (word[*i] && ((ft_isalnum(word[*i]) || ft_isspecial(word[*i]))))
	{
		if (word[*i] == '*' && *i > start)
			break;
		(*i)++;
	}
	var_name = ft_substr(word, start, *i - start - 1);
	value = get_env_value(all, var_name);
	free(var_name);
	return (value);
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

char	*expand_word(t_all *all, char *word)
{
	int i;
	char *new_word;
	char *expansion;

	i = 0;
	new_word = ft_strdup("");
	while (word[i])
	{
		if (word[i] == '$' && word[i + 1])
		{
			if (word[i + 1] == '?')
				expansion = expand_status(all, &i);
			else if (ft_isdigit(word[i + 1]))
				expansion = expand_digit(&i);
			else
				expansion = expand_variable(all, word, &i);
			new_word = ft_strjoin(new_word, expansion, 1, 1);
		}
		else
			new_word = ft_strjoinchar(new_word, word[i++]);
	}
	return (new_word);
}

// char	*expand_wildcard(char *string)
// {
	
// }

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
			new = token_new(tmp->token_type, expand_word(all, tmp->value), tmp->single_quoted, tmp->double_quoted);
		else
			new = token_new(tmp->token_type, ft_strdup(tmp->value), tmp->single_quoted, tmp->double_quoted);
		new->space_after = tmp->space_after;
		token_addback(&expanded_tokens, new);
		tmp = tmp->next;
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