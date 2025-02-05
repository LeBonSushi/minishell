/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <maxence22corso@gmail.com>         +#+  +:+       +#+        */
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
	(*token)->token_type = type;
	if ((*token)->value)
		free((*token)->value);
	(*token)->value = value;
}

t_token	*ft_parse_operator(char **str)
{
	t_token			*token;

	token = token_new(WORD, NULL, false, false);
	if (**str == ';')
		edit_token(&token, SEMICOLON, ft_strdup(";"));
	else if (**str == '|')
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
	else
		return (NULL);
	(*str)++;
	return (token);
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

bool	is_valid_tokens(t_all *all, t_token *token)
{
	if (!token)
		return (true);
	while (token)
	{
		if (token->token_type == REDIR_IN || token->token_type == REDIR_OUT
			|| token->token_type == REDIR_APPEND)
		{
			if (!token->next)
			{
				all->status = 2;
				printf("ministress: syntax error near unexpected token `newline\'\n");
				return (false);
			}
			if (token->next->token_type != WORD)
			{
				all->status = 2;
				printf("ministress: syntax error near unexpected token `%s\'\n", token->next->value);
				return (false);
			}
		}
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
	t_token *tmp = *tokens;
	t_token *next;

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
