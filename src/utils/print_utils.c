/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 23:16:19 by macorso           #+#    #+#             */
/*   Updated: 2025/02/05 01:05:12 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		printf("%s\n", tab[i]);
		i++;
	}
}

void	print_list(t_string_list *list)
{
	t_string_list *curr;

	curr = list;
	while (curr)
	{
		if (curr->redir_type == REDIR_IN)
			printf("TYPE: REDIR_IN\n");
		if (curr->redir_type == REDIR_OUT)
			printf("TYPE: REDIR_OUT\n");
		if (curr->redir_type == REDIR_APPEND)
			printf("TYPE: REDIR_APPEND\n");
		if (curr->redir_type == REDIR_HEREDOC)
			printf("TYPE: REDIR_HEREDOC\n");
		printf("%s\n", curr->str);
		curr = curr->next;
	}
}

void	print_tokens(t_token *tokens)
{
	t_token	*tmp;
	tmp = tokens;
	while (tmp)
	{
		if (tmp->token_type == WORD)
			printf("WORD\n(%s)\n", tmp->value);
		else if (tmp->token_type == SEMICOLON)
			printf("SEMICOLON\n%s\n", tmp->value);
		else if (tmp->token_type == PIPE)
			printf("PIPE\n%s\n", tmp->value);
		else if (tmp->token_type == REDIR_IN)
			printf("REDIR_IN\n%s\n", tmp->value);
		else if (tmp->token_type == REDIR_OUT)
			printf("REDIR_OUT\n%s\n", tmp->value);
		else if (tmp->token_type == REDIR_APPEND)
			printf("REDIR_APPEND\n%s\n", tmp->value);
		else if (tmp->token_type == REDIR_HEREDOC)
			printf("REDIR_HEREDOC\n%s\n", tmp->value);
		else if (tmp->token_type == WILD_CARD)
			printf("WILD_CARD\n%s\n", tmp->value);
		else
			printf("ERROR\n%s\n", tmp->value);
		printf("single quoted: %d\n", tmp->single_quoted);
		printf("double quoted: %d\n\n", tmp->double_quoted);
		printf("space after: %s\n\n", tmp->space_after ? "true" : "false");
		tmp = tmp->next;
	}
}