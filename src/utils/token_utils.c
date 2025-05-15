/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jd-halle <jd-halle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 16:00:14 by macorso           #+#    #+#             */
/*   Updated: 2025/05/15 09:42:32 by jd-halle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

bool	is_operator(char *str)
{
	if (*str == '|' || *str == '<' || *str == '>' || *str == '~')
		return (true);
	return (false);
}

bool	is_same_quote(t_token *token, t_token *token_to_compare)
{
	if (!token || !token_to_compare)
		return (false);
	if (token->single_quoted == token_to_compare->single_quoted
		&& token->double_quoted == token_to_compare->double_quoted)
		return (true);
	return (false);
}

bool	is_same_type(t_token *token, t_token *token_to_compare,
	t_token_type type)
{
	if (!token || !token_to_compare)
		return (false);
	if (token->token_type == type && token_to_compare->token_type == type)
		return (true);
	return (false);
}

char	*join_parsed_tokens(t_token *tokens)
{
	t_token	*curr;
	char	*result;
	char	*tmp;

	if (!tokens)
		return (ft_strdup(""));
	result = ft_strdup(tokens->value);
	curr = tokens->next;
	while (curr)
	{
		if (curr->space_after)
		{
			tmp = ft_strjoin(result, " ", 1, 0);
			if (!tmp)
				return (NULL);
			result = tmp;
		}
		tmp = ft_strjoin(result, curr->value, 1, 0);
		if (!tmp)
			return (NULL);
		result = tmp;
		curr = curr->next;
	}
	return (result);
}
