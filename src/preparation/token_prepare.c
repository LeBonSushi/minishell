/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_prepare.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 05:02:03 by macorso           #+#    #+#             */
/*   Updated: 2025/05/24 16:26:04 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*token_new(t_token_type token_type, char *value,
			bool single_q, bool double_q)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->token_type = token_type;
	if (value)
		new->value = ft_strdup(value);
	new->single_quoted = single_q;
	new->double_quoted = double_q;
	new->space_after = true;
	new->expanded = false;
	new->next = NULL;
	return (new);
}

void	token_addback(t_token **lst, t_token *new)
{
	t_token	*tmp;

	if (!lst)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	tmp = *lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}
