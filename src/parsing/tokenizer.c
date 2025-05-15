/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 23:39:59 by macorso           #+#    #+#             */
/*   Updated: 2025/02/05 03:27:24 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*ft_parse_quoted(char **str, bool *sq, bool *dq)
{
	char	quote;
	char	*start;
	int		len;
	t_token	*token;
	char	*sub;

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
	sub = ft_substr(start, 0, len);
	token = token_new(WORD, sub, *sq, *dq);
	free(sub);
	token->space_after = (ft_isspace(*(*str + 1)));
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
	t_token	*token;

	token = NULL;
	if (**str == '|')
		edit_token(&token, PIPE, ft_strdup("|"));
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
	else if (**str == '~')
		edit_token(&token, TILDE, ft_strdup("~"));
	return (token);
}

t_token	*ft_parse_operator(char **str)
{
	t_token	*token;

	token = verif_and_edit_operator(str);
	(*str)++;
	if ((*str) && ft_isspace(**str))
		token->space_after = true;
	return (token);
}

t_token	*ft_parse_word(char **str, bool sq, bool dq)
{
	char	*start;
	int		len;
	bool	space_after;
	t_token	*token;
	char	*word;

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
	word = ft_substr(start, 0, len);
	token = token_new(WORD, word, false, false);
	free(word);
	token->space_after = space_after;
	return (token);
}
