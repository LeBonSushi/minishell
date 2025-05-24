/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 11:12:54 by macorso           #+#    #+#             */
/*   Updated: 2025/05/24 16:45:52 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
