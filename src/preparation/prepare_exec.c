/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 14:35:21 by macorso           #+#    #+#             */
/*   Updated: 2025/05/11 03:01:37 by macorso          ###   ########.fr       */
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
	size_t	pipes;
	t_token	*curr;

	pipes = 1;
	curr = token;
	if (!token)
		return (0);
	while (curr)
	{
		if (curr->token_type == PIPE)
			pipes++;
		curr = curr->next;
	}
	return (pipes);
}

// t_token	**split_tokens(t_token *tokens)
// {
// 	t_token	**split;
// 	t_token	*curr;
// 	t_token	*to_add;
// 	t_token	*next;
// 	int		i;

// 	if (!tokens)
// 		return (NULL);
// 	curr = tokens;
// 	i = 0;
// 	split = malloc(sizeof(t_token *) * (count_commands(tokens) + 2));
// 	printf("%lu\n", count_commands(tokens) + 1);
// 	if (!split)
// 		return (NULL);
// 	while (curr)
// 	{
// 		to_add = curr;
// 		while (curr && curr->token_type != PIPE)
// 			curr = curr->next;
// 		if (curr && curr->token_type == PIPE)
// 		{	
// 			next = curr->next;
// 			free_token(curr);
// 			curr = next;
// 			continue ;
// 		}
// 		split[i] = to_add;
// 		i++;
// 	}
// 	split[i] = NULL;
// 	return (split);
// }

t_token **split_tokens(t_token *tokens)
{
    t_token **split;
    t_token *curr;
    t_token *cmd_start;
    t_token *prev = NULL;
    size_t i = 0;
    size_t cmd_count;

    if (!tokens)
        return NULL;

    cmd_count = count_commands(tokens);
    split = malloc(sizeof(t_token *) * (cmd_count + 1));
    if (!split)
        return NULL;

    curr = tokens;
    cmd_start = curr;
    
    while (curr)
    {
        if (curr->token_type == PIPE)
        {
            // Save the start of the current command
            split[i++] = cmd_start;
            
            // Terminate the current command list
            if (prev)
                prev->next = NULL;
            
            // Move to next token (after PIPE)
            t_token *pipe_to_free = curr;
            curr = curr->next;
            free_token(pipe_to_free);
            
            // New command starts here
            cmd_start = curr;
            prev = NULL;
            continue;
        }
        
        prev = curr;
        curr = curr->next;
    }
    
    // Add the last command if it exists
    if (cmd_start)
        split[i++] = cmd_start;
    
    split[i] = NULL;  // NULL-terminate the array
    return split;
}

t_cmd	*create_cmdlist(t_token *token)
{
	t_cmd	*cmds;
	t_token	**split;
	int	i;
	
	split = split_tokens(token);
	// print_tokens(*split);
	cmds = NULL;
	if (!split)
		return (NULL);
	i = -1;
	while (split[++i])
		cmd_addback(&cmds, cmd_new(split[i]));
	free(split);
	return (cmds);
}
