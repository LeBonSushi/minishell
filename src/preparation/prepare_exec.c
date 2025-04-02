/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 14:35:21 by macorso           #+#    #+#             */
/*   Updated: 2025/03/31 07:06:56 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*copy_token2(t_token *src)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->token_type = src->token_type;
	new->single_quoted = src->single_quoted;
	new->double_quoted = src->double_quoted;
	new->space_after = src->space_after;
	new->value = ft_strdup(src->value);
	new->next = NULL;
	return (new);
}

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
		new = copy_token2(curr);
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

t_cmd	*create_cmdlist(t_token *token)
{
	t_cmd	*head;
	t_token	*curr;
	t_token	*cmd_start;
	t_cmd	*new_cmd;
	t_token	*tokens_copy;

	head = NULL;
	curr = token;
	cmd_start = token;
	while (curr)
	{
		if (curr->token_type == PIPE)
		{
			tokens_copy = copy_token_list(cmd_start, curr);
			new_cmd = cmd_new(tokens_copy);
			cmd_addback(&head, new_cmd);
			cmd_start = curr->next;
		}
		curr = curr->next;
	}
	if (cmd_start)
	{
		tokens_copy = copy_token_list(cmd_start, NULL);
		new_cmd = cmd_new(tokens_copy);
		cmd_addback(&head, new_cmd);
	}
	return (head);
}