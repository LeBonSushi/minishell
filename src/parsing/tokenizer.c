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
	t_token			*token;

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
	char	*word = ft_substr(start, 0, len);
	token = token_new(WORD, word, false, false);
	free(word);
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
	(void)all;
	if (is_redirect(token->token_type))
	{
		if (!token->next)
		{
			g_zebi.g_exit = 2;
			ft_putstr_fd("ministress: syntax error near unexpected token `newline\'\n", 2);
			return (true);
		}
		if (token->next->token_type != WORD && token->next->token_type != WILD_CARD)
		{
			g_zebi.g_exit = 2;
			printf("ministress: syntax error near unexpected token `%s\'\n", token->next->value);
			return (true);
		}
		if (!ft_strcmp(token->next->value, "*") && is_redirect(token->token_type))
		{
			g_zebi.g_exit = 1;
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
	(void)all;

	i = 0;
	if (token->token_type == WORD)
	{
		while (token->value[i])
		{
			if ((token->value[i] == '$') && is_error_expansion_char(token->value[i + 1]) && !token->value[i + 2])
			{
				g_zebi.g_exit = 2;
				printf("ministress: syntax error near unexpected token `%c\'\n", token->value[i + 1]);
				return (true);
			}
			i++;
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

void	delete_token2(t_token **tokens, t_token *token_to_del)
{
	t_token	*tmp;
	t_token	*prev;

	if (!tokens || !*tokens || !token_to_del)
		return;
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
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

void	delete_token(t_token **head_ref, t_token *node_to_delete)
{
	t_token	*current;

	if (!head_ref || !*head_ref || !node_to_delete)
		return ;
	if (*head_ref == node_to_delete)
	{
		*head_ref = node_to_delete->next;
		free(node_to_delete->value);
		free(node_to_delete);
		return ;
	}
	current = *head_ref;
	while (current && current->next != node_to_delete)
		current = current->next;
	if (!current)
		return ;
	current->next = node_to_delete->next;
	free(node_to_delete->value);
	free(node_to_delete);
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

char	get_unclose_quote(char *str)
{
	int	i;
	bool	sq;
	bool	dq;

	i = 0;
	sq = false;
	dq = false;
	while (str[i])
	{
		if (str[i] == '\'' && !dq)
			sq = !sq;
		if (str[i] == '\"' && !sq)
			dq = !dq;
		i++;
	}
	if (dq)
		return ('"');
	return ('\'');
}

char *manage_unclosed_quote(char *str, char quote)
{
    char *line;
    char *result;
    char *tmp;
    char *combined_str;

    // Ajouter un saut de ligne à la chaîne originale
    result = ft_strjoin(str, "\n", 1, 0);
    if (!result)
        return (NULL);
    
    while (1)
    {
        if (quote == '"')
            line = readline("dquote> ");
        else
            line = readline("quote> ");
        if (!line)
            break;
        tmp = ft_strjoin(line, "\n", 0, 0);
        if (!tmp)
        {
            free(line);
            free(result);
            return (NULL);
        }
        combined_str = ft_strjoin(result, tmp, 1, 1);
        if (!combined_str)
        {
            free(line);
            return (NULL);
        }
        result = combined_str;
        if (is_quotes_closed(result))
        {
            free(line);
            break;
        }
        free(line);
    }
    return (result);
}

t_token	*fill_tokens(char *str)
{
	t_token	*tokens;
	char	*tmp;
	char	*result;

	if (!str)
		return (NULL);
	if (!is_quotes_closed(str))
	{
		tmp = str;
		str = manage_unclosed_quote(tmp, get_unclose_quote(tmp));
	}

	tokens = NULL;
	while (*str)
		token_addback(&tokens, new_parsed_token(&str));
	// manage_empty_tokens(&tokens);
	// if (!is_valid_tokens(all, tokens))
	// 	return (free_all_token(tokens), NULL);
	return (tokens);
}
