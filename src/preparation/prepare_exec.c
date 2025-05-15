/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jd-halle <jd-halle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 14:35:21 by macorso           #+#    #+#             */
/*   Updated: 2025/05/15 04:37:19 by jd-halle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*copy_token(t_token *src)
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
		new = copy_token(curr);
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

static void	add_cmd_to_list(t_token *cmd_start, t_token *cmd_end, t_cmd **head)
{
	t_token	*tokens_copy;
	t_cmd	*new_cmd;

	tokens_copy = copy_token_list(cmd_start, cmd_end);
	new_cmd = cmd_new(tokens_copy);
	cmd_addback(head, new_cmd);
}

t_cmd	*create_cmdlist(t_token *token)
{
	t_cmd	*head;
	t_token	*curr;
	t_token	*cmd_start;

	head = NULL;
	curr = token;
	cmd_start = token;
	while (curr)
	{
		if (curr->token_type == PIPE)
		{
			add_cmd_to_list(cmd_start, curr, &head);
			cmd_start = curr->next;
		}
		curr = curr->next;
	}
	if (cmd_start)
		add_cmd_to_list(cmd_start, NULL, &head);
	return (head);
}
