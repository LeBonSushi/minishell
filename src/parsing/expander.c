/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jd-halle <jd-halle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 04:28:23 by macorso           #+#    #+#             */
/*   Updated: 2025/04/29 21:56:41 by jd-halle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool is_same_type(t_token *token, t_token *token_to_compare, t_token_type type)
{
	if (!token || !token_to_compare)
		return (false);
	if (token->token_type == type && token_to_compare->token_type == type)
		return (true);
	return (false);
}
bool is_same_quote(t_token *token, t_token *token_to_compare)
{
	if (!token || !token_to_compare)
		return (false);
	if (token->single_quoted == token_to_compare->single_quoted
		&& token->double_quoted == token_to_compare->double_quoted)
		return (true);
	return (false);
}

char *join_token_values(t_token *first, t_token *second)
{
	char *joined;
	char *temp;

	temp = ft_strdup(first->value);
	joined = ft_strjoin(temp, second->value, 1, 0);
	return (joined);
}

t_token	*token_new2(t_token_type token_type, char *value,
			bool single_q, bool double_q)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->token_type = token_type;
	new->value = value;
	new->single_quoted = single_q;
	new->double_quoted = double_q;
	new->space_after = true;
	new->next = NULL;
	return (new);
}

t_token	*join_tokens(t_token *tmp, t_token *next, t_token **next_ptr)
{
	t_token	*new;
	char	*joined_value;

	joined_value = join_token_values(tmp, next);
	new = token_new(tmp->token_type, joined_value,
			tmp->single_quoted, tmp->double_quoted);
	new->space_after = next->space_after;
	*next_ptr = next->next;
	return (new);
}
