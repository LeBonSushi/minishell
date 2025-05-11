/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander5.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jd-halle <jd-halle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:57:26 by jd-halle          #+#    #+#             */
/*   Updated: 2025/04/29 22:59:16 by jd-halle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

void init_tokens(bool *merged, t_token **curr, t_token *tokens)
{
	*merged = false;
	*curr = tokens;
}
