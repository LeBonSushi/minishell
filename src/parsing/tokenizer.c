/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 23:39:59 by macorso           #+#    #+#             */
/*   Updated: 2025/02/03 20:45:44 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*ft_parse_quoted(char **str, bool *sq, bool *dq)
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
	token = token_new(WORD, ft_substr(start, 0, len),
			*sq, *dq);
	printf("sq: %d, dq: %d\n", *sq, *dq);
	(*str)++;
	return (token);
}

t_token	*ft_parse_operator(char **str)
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
	return (token_new(type, NULL, false, false));
}

t_token	*ft_parse_word(char **str, bool sq, bool dq)
{
	char	*start;
	int		len;

	start = *str;
	len = 0;
	while (**str && !ft_isspace(**str) && !is_operator(**str)
		&& (!is_quote(**str) || sq || dq))
	{
		(*str)++;
		len++;
	}
	return (token_new(WORD, ft_substr(start, 0, len), false, false));
}

t_token	*new_token(char	**str)
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
	if (is_operator(**str))
		new_token = ft_parse_operator(str);
	else if (is_quote(**str) && !sq && !dq)
		new_token = ft_parse_quoted(str, &sq, &dq);
	else
		new_token = ft_parse_word(str, sq, dq);
	if (!new_token)
		return (NULL);
	return (new_token);
}

t_token	*fill_tokens(char *str)
{
	t_token	*head;

	if (!str)
		return (NULL);
	head = NULL;
	while (*str)
		token_addback(&head, new_token(&str));
	return (head);
}
