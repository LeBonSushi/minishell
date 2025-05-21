/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:55:54 by jd-halle          #+#    #+#             */
/*   Updated: 2025/05/21 11:13:07 by macorso          ###   ########.fr       */
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

t_token	*create_expand_tokens(t_all *all, t_token *tokens)
{
	t_token	*expanded_tokens;

	if (!tokens)
		return (NULL);
	expanded_tokens = expand_tokens(all, tokens);
	free_all_token(tokens);
	return (expanded_tokens);
}
