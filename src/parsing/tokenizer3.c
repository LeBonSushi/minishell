/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jd-halle <jd-halle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 09:46:54 by jd-halle          #+#    #+#             */
/*   Updated: 2025/05/15 09:47:16 by jd-halle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	manage_expansion_error(t_token *token)
{
	int	i;

	i = 0;
	if (token->token_type == WORD)
	{
		while (token->value[i])
		{
			if ((token->value[i] == '$')
				&& is_error_expansion_char(token->value[i + 1])
				&& !token->value[i + 2])
			{
				g_zebi.g_exit = 2;
				printf("ministress: syntax error near unexpected token `%c\'\n",
					token->value[i + 1]);
				return (true);
			}
			i++;
		}
	}
	return (false);
}

bool	is_valid_tokens(t_token *token)
{
	if (!token)
		return (true);
	while (token)
	{
		if (manage_error_redir(token))
			return (false);
		if (manage_expansion_error(token))
			return (false);
		token = token->next;
	}
	return (true);
}

void	delete_token2(t_token **tokens, t_token *token_to_del)
{
	t_token	*tmp;
	t_token	*prev;

	if (!tokens || !*tokens || !token_to_del)
		return ;
	tmp = *tokens;
	prev = NULL;
	while (tmp)
	{
		if (tmp == token_to_del)
		{
			if (prev)
				prev->next = tmp->next;
			else
				*tokens = tmp->next;
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

void	delete_token(t_token **head_ref, t_token *node_to_delete)
{
	t_token	*current;

	if (!head_ref || !*head_ref || !node_to_delete)
		return ;
	if (*head_ref == node_to_delete)
	{
		*head_ref = node_to_delete->next;
		free(node_to_delete->value);
		free(node_to_delete);
		return ;
	}
	current = *head_ref;
	while (current && current->next != node_to_delete)
		current = current->next;
	if (!current)
		return ;
	current->next = node_to_delete->next;
	free(node_to_delete->value);
	free(node_to_delete);
}

void	manage_empty_tokens(t_token **tokens)
{
	t_token	*tmp;
	t_token	*next;

	if (!tokens || !*tokens)
		return ;
	tmp = *tokens;
	while (tmp)
	{
		next = tmp->next;
		if (tmp->token_type == WORD && !ft_strcmp(tmp->value, ""))
		{
			if (!tmp->space_after)
				delete_token(tokens, tmp);
		}
		tmp = next;
	}
	tmp = *tokens;
	if (!tmp || !tmp->next)
		return ;
	while (tmp->next)
		tmp = tmp->next;
	tmp->space_after = false;
}
