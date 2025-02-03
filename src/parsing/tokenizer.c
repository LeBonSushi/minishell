/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 23:39:59 by macorso           #+#    #+#             */
/*   Updated: 2025/02/03 18:16:48 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	ft_is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

static bool	ft_is_operator(char c)
{
	return (c == ';' || c == '|' || c == '<' || c == '>');
}

static t_token	*ft_create_token(t_token_type type, char *value, bool sq, bool dq)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->token_type = type;
	token->value = value;
	token->single_quoted = sq;
	token->double_quoted = dq;
	token->next = NULL;
	return (token);
}

static t_token	*ft_parse_quoted(char **str, bool *sq, bool *dq)
{
	char	quote;
	char	*start;
	int		len;
	t_token	*token;

	quote = **str;
	(*str)++;
	start = *str;
	len = 0;
	while (**str && **str != quote)
	{
		(*str)++;
		len++;
	}
	if (**str != quote)
		return (NULL);
	*sq = (quote == '\'');
	*dq = (quote == '\"');
	token = ft_create_token(WORD, ft_substr(start, 0, len),
			*sq, *dq);
	printf("sq: %d, dq: %d\n", *sq, *dq);
	(*str)++;
	return (token);
}

static t_token	*ft_parse_operator(char **str)
{
	t_token_type	type;

	if (**str == ';')
		type = SEMICOLON;
	else if (**str == '|')
		type = PIPE;
	else if (**str == '<')
		type = REDIR_IN;
	else if (**str == '>')
	{
		if (*(*str + 1) == '>')
		{
			(*str)++;
			type = REDIR_APPEND;
		}
		else
			type = REDIR_OUT;
	}
	else
		return (NULL);
	(*str)++;
	return (ft_create_token(type, NULL, false, false));
}

static t_token	*ft_parse_word(char **str, bool sq, bool dq)
{
	char	*start;
	int		len;

	start = *str;
	len = 0;
	while (**str && !ft_isspace(**str) && !ft_is_operator(**str)
		&& (!ft_is_quote(**str) || sq || dq))
	{
		(*str)++;
		len++;
	}
	return (ft_create_token(WORD, ft_substr(start, 0, len), false, false));
}

// t_token	*get_new_token(char *str, bool sq, bool dq)
// {
// 	t_token	*new_token;
	
	
// 	return (new_token);
// }

t_token	*fill_tokens(char *str)
{
	t_token	*head;
	t_token	*current;
	bool	sq;
	bool	dq;
	t_token	*new_token;

	if (!str)
		return (NULL);
	head = NULL;
	current = NULL;
	sq = false;
	dq = false;
	new_token = NULL;
	while (*str)
	{
		while (ft_isspace(*str))
			str++;
		if (!*str)
			break ;
		if (ft_is_operator(*str))
			new_token = ft_parse_operator(&str);
		else if (ft_is_quote(*str) && !sq && !dq)
			new_token = ft_parse_quoted(&str, &sq, &dq);
		else
			new_token = ft_parse_word(&str, sq, dq);
		if (!new_token)
			break ;
		if (!head)
			head = (current = new_token);
		else
			current = (current->next = new_token);
	}
	return (head);
}
