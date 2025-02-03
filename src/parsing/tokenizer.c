/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 23:39:59 by macorso           #+#    #+#             */
/*   Updated: 2025/02/03 15:55:53 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	is_operator(char c)
{
	return (c == ';' || c == '|' || c == '<' || c == '>' || ft_isspace(c));
}

t_token_type	token_type_get(char *str, int *i)
{
	if (str[*i] == ';')
		return (SEMICOLON);
	else if (str[*i] == '|')
		return (PIPE);
	else if (str[*i] == '<')
		return (REDIR_IN);
	else if (str[*i] == '>')
	{
		if (str[(*i) + 1] == '>')
		{
			(*i)++;
			return (REDIR_APPEND);
		}
		return (REDIR_OUT);
	}
	else
	{
		printf("Unknown token type\n");
		return (0);
	}
}

// t_token	*fill_tokens(char *str)
// {
// 	t_token	*tokens;
// 	int		i;
// 	bool	single_quote_open;
// 	bool	double_quote_open;
// 	int		j;

// 	if (!str)
// 		return (NULL);
// 	i = -1;
// 	j = 0;
// 	single_quote_open = false;
// 	double_quote_open = false;
// 	tokens = NULL;
// 	while (str[++i])
// 	{
		
// 		if (str[i] == ';' || str[i] == '|' || str[i] == '<' || str[i] == '>') 
// 			token_addback(&tokens, token_new(token_type_get(str, &i), NULL, false, false));
// 		else if (!single_quote_open && !double_quote_open && ft_isspace(str[i]))
// 			continue ;
// 		else if (str[i] == '\'' || str[i] == '\"')
// 		{
// 			if (str[i] == '\'' && !double_quote_open)
// 				single_quote_open = !single_quote_open;
// 			else if (str[i] == '\"' && !single_quote_open)
// 				double_quote_open = !double_quote_open;
// 			j = i;
// 			while ((single_quote_open || double_quote_open) && str[++i])
// 			{
// 				if ((str[i] == '\'' && single_quote_open) || (str[i] == '\"' && double_quote_open))
// 				{
// 					token_addback(&tokens, token_new(WORD, ft_substr(str, j + 1, i - j - 1), single_quote_open, double_quote_open));
// 					single_quote_open = false;
// 					double_quote_open = false;
// 				}
// 			}
// 		}
// 		else
// 		{
// 			j = i;
// 			while (!is_operator(str[i + 1]) && str[i + 1])
// 				i++;
// 			token_addback(&tokens, token_new(WORD, ft_substr(str, j, i - j + 1), false, false));
// 		}
// 	}
// 	return (tokens);
// }



static t_token	*handle_quote_token(char *str, bool *single_quote, bool *double_quote)
{
	t_token	*token;
	char	quote;
	int		len;

	quote = *str;
	str++;
	len = 0;
	while (str[len] && str[len] != quote)
		len++;
	token = token_new(WORD, ft_substr(str, 0, len), 
		(quote == '\''), (quote == '\"'));
	*single_quote = (quote == '\'');
	*double_quote = (quote == '\"');
	return (token);
}

static t_token	*handle_word_token(char *str)
{
	int		len;
	t_token	*token;

	len = 0;
	while (str[len] && !ft_isspace(str[len]) 
		&& !is_operator(str[len]) 
		&& str[len] != '\'' 
		&& str[len] != '\"')
		len++;
	token = token_new(WORD, ft_substr(str, 0, len), false, false);
	return (token);
}

static t_token	*fill_tokens_recursive(char *str, bool sq, bool dq)
{
	t_token	*tokens;

	if (!str || *str == '\0')
		return (NULL);
	if (*str == ';' || *str == '|' || *str == '<' || *str == '>')
		return (token_new(token_type_get(str, NULL), NULL, false, false));
	if (!sq && !dq && ft_isspace(*str))
		return (fill_tokens_recursive(str + 1, sq, dq));
	if ((*str == '\'' && !dq) || (*str == '\"' && !sq))
	{
		tokens = handle_quote_token(str, &sq, &dq);
		tokens->next = fill_tokens_recursive(str + ft_strlen(tokens->value) + 2, 
			false, false);
		return (tokens);
	}
	tokens = handle_word_token(str);
	tokens->next = fill_tokens_recursive(str + ft_strlen(tokens->value), sq, dq);
	return (tokens);
}

t_token	*fill_tokens(char *str)
{
	if (!str)
		return (NULL);
	return (fill_tokens_recursive(str, false, false));
}