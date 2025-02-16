/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 04:28:23 by macorso           #+#    #+#             */
/*   Updated: 2025/02/15 02:42:29 by macorso          ###   ########.fr       */
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