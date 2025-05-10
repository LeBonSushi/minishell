/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 14:35:21 by macorso           #+#    #+#             */
/*   Updated: 2025/05/10 02:38:42 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*copy_token3(t_token *src)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->token_type = src->token_type;
	new->single_quoted = src->single_quoted;
	new->double_quoted = src->double_quoted;
	new->space_after = src->space_after;
	new->value = src->value;
	new->next = NULL;
	return (new);
}

t_token	*copy_token_list(t_token *start, t_token *end)
{
	t_token	*head;
	t_token	*curr;
	t_token	*new;
	t_token	*prev;

	head = NULL;
	prev = NULL;
	curr = start;
	while (curr && curr != end)
	{
		new = copy_token3(curr);
		if (!new)
			return (NULL);
		if (!head)
			head = new;
		else
			prev->next = new;
		prev = new;
		curr = curr->next;
	}
	return (head);
}

void cmd_addback(t_cmd **cmds, t_cmd *new)
{
	t_cmd *curr;

	if (!cmds || !new)
		return ;
	if (!*cmds)
	{
		*cmds = new;
		return ;
	}
	while (curr->next)
		curr = curr->next;
	
}

t_cmd	*create_cmdlist(t_token *token)
{
	t_cmd	*head;
	
	
}