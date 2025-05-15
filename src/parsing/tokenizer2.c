/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jd-halle <jd-halle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 09:45:57 by jd-halle          #+#    #+#             */
/*   Updated: 2025/05/15 09:46:51 by jd-halle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*new_parsed_token(char **str)
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
	return (type == REDIR_IN || type == REDIR_OUT || type == REDIR_APPEND
		|| type == REDIR_HEREDOC);
}

bool	manage_error_redir(t_token *token)
{
	if (is_redirect(token->token_type))
	{
		if (!token->next)
		{
			g_zebi.g_exit = 2;
			ft_putstr_fd("ministress: syntax error near unexpected \
token `newline\'\n", 2);
			return (true);
		}
		if (token->next->token_type != WORD
			&& token->next->token_type != WILD_CARD)
		{
			g_zebi.g_exit = 2;
			printf("ministress: syntax error near unexpected token `%s\'\n",
				token->next->value);
			return (true);
		}
		if (!ft_strcmp(token->next->value, "*")
			&& is_redirect(token->token_type))
		{
			g_zebi.g_exit = 1;
			return (ft_putstr_fd("ministress: *: ambiguous redirect\n", 2),
				true);
		}
	}
	return (false);
}

bool	is_error_expansion_char(char c)
{
	return (c == ')' || c == '(' || c == '{' || c == '-' || c == '#');
}
