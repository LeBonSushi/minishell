/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 14:35:21 by macorso           #+#    #+#             */
/*   Updated: 2025/05/10 05:27:59 by macorso          ###   ########.fr       */
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

size_t	count_commands(t_token *token)
{
	size_t	commands;
	t_token	*curr;

	commands = 0;
	curr = token;
	bool was_pipe;

	was_pipe = false;
	if (!token)
		return (0);
	while (curr)
	{
		if (curr->token_type != PIPE)
		{
			if (was_pipe)
				commands++;
			was_pipe = false;
		}
		else
			was_pipe = true;
		curr = curr->next;
	}
	return (commands);
}

t_token	**split_tokens(t_token *tokens)
{
	t_token	**split;
	t_token	*curr;
	t_token	*to_add;
	t_token	*next;
	int		i;

	if (!tokens)
		return (NULL);
	curr = tokens;
	i = 0;
	split = malloc(sizeof(t_token *) * (count_commands(tokens) + 1));
	if (!split)
		return (NULL);
	while (curr)
	{
		to_add = curr;
		while (curr && curr->token_type != PIPE)
			curr = curr->next;
		if (curr && curr->token_type == PIPE)
		{
			next = curr->next;
			free(curr->value);
			free(curr->next);
			free(curr);
			curr = next;
		}
		split[i] = to_add;
		i++;
	}
	split[i] = NULL;
	return (split);
}

t_cmd	*create_cmdlist(t_token *token)
{
	t_cmd	*cmds;
	t_token	**split;
	int	i;
	
	split = split_tokens(token);
	cmds = NULL;
	if (!split)
		return (NULL);
	i = -1;
	while (split[++i])
		;
	cmds = malloc(sizeof(t_cmd) + (i + 1));
	i = -1;
	while (split[++i])
		cmd_addback(&cmds, cmd_new(split[i]));
	return (cmds);
}
