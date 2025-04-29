/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander6.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jd-halle <jd-halle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:58:30 by jd-halle          #+#    #+#             */
/*   Updated: 2025/04/29 22:57:27 by jd-halle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*merge_tokens(t_token *tokens)
{
	t_token	*curr;
	t_token	*next;
	bool	merged;

	if (!tokens || !tokens->next)
		return (tokens);
	init_tokens(&merged, &curr, tokens);
	while (curr && curr->next)
	{
		if (!is_redir(curr) && !is_redir(curr->next) && curr->token_type != PIPE
			&& curr->next->token_type != PIPE && !curr->space_after)
		{
			curr->value = join_token_values(curr, curr->next);
			next = curr->next;
			curr->next = curr->next->next;
			curr->space_after = next->space_after;
			free_token(next);
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
	t_token *token_after;

	if (!tokens)
		return (NULL);
	token_after = create_tokens_without_space(tokens);
	expanded_tokens = expand_tokens(all, token_after);
	free_all_token(token_after);
	return (expanded_tokens);
}
