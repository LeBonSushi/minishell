/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jd-halle <jd-halle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:55:54 by jd-halle          #+#    #+#             */
/*   Updated: 2025/05/15 11:08:22 by jd-halle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// t_token	*expand_tokens(t_all *all, t_token *tokens)
// {
// 	t_token	*tmp;
// 	t_token	*expanded_tokens;
// 	t_token	*new;

// 	expanded_tokens = NULL;
// 	tmp = tokens;
// 	while (tmp)
// 	{
// 		new = handle_token_expansion(all, tmp);
// 		if (!new)
// 		{
// 			free_all_token(expanded_tokens);
// 			return (NULL);
// 		}
// 		if (!ft_strcmp(new->value, "") && new->expanded == true)
// 		{
// 			free(new->value);
// 			free(new);
// 			tmp = tmp->next;
// 			continue ;
// 		}
// 		new->space_after = tmp->space_after;
// 		if (!*new->value && ft_strcmp(tmp->value, ""))
// 			new->space_after = false;
// 		token_addback(&expanded_tokens, new);
// 		tmp = tmp->next;
// 	}
// 	return (expanded_tokens);
// }

static void	handle_empty_token(t_token *new, t_token **tmp)
{
	free(new->value);
	free(new);
	*tmp = (*tmp)->next;
}

static void	adjust_token_space(t_token *new, t_token *tmp)
{
	new->space_after = tmp->space_after;
	if (!*new->value && ft_strcmp(tmp->value, ""))
		new->space_after = false;
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
			return (NULL);
		}
		if (!ft_strcmp(new->value, "") && new->expanded == true)
		{
			handle_empty_token(new, &tmp);
			continue ;
		}
		adjust_token_space(new, tmp);
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

static void	init_tokens(bool *merged, t_token **curr, t_token *tokens)
{
	*merged = false;
	*curr = tokens;
}

t_token	*merge_tokens(t_token *tokens)
{
	t_token	*curr;
	bool	merged;

	if (!tokens || !tokens->next)
		return (tokens);
	init_tokens(&merged, &curr, tokens);
	while (curr && curr->next)
	{
		if (!is_redir(curr) && !is_redir(curr->next) && curr->token_type != PIPE
			&& curr->next->token_type != PIPE && !curr->space_after)
		{
			curr->value = ft_strjoin(curr->value, curr->next->value, 1, 0);
			curr->space_after = curr->next->space_after;
			delete_token(&tokens, curr->next);
			merged = true;
			break ;
		}
		curr = curr->next;
	}
	if (merged)
		return (merge_tokens(tokens));
	return (tokens);
}

t_token	*create_expand_tokens(t_all *all, t_token *tokens)
{
	t_token	*expanded_tokens;

	if (!tokens)
		return (NULL);
	expanded_tokens = expand_tokens(all, tokens);
	free_all_token(tokens);
	return (expanded_tokens);
}
