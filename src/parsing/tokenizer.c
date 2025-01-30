/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 23:39:59 by macorso           #+#    #+#             */
/*   Updated: 2025/01/30 02:25:40 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	is_operator(char c)
{
	return (c == ';' || c == '|' || c == '<' || c == '>' || ft_isspace(c));
}

t_token_type	token_type_get(char *str, int *i)
{
	if (str[*i] == ';')
		return (SEMICOLON);
	else if (str[*i] == '|')
		return (PIPE);
	else if (str[*i] == '<')
		return (REDIR_IN);
	else if (str[*i] == '>')
	{
		if (str[(*i) + 1] == '>')
		{
			(*i)++;
			return (REDIR_APPEND);
		}
		return (REDIR_OUT);
	}
	else
	{
		printf("Unknown token type\n");
		return (0);
	}
}

t_token	*fill_tokens(char *str)
{
	t_token	*tokens;
	int		i;
	int		j;

	if (!str)
		return (NULL);
	i = -1;
	j = 0;
	tokens = NULL;
	while (str[++i])
	{
		
		if (str[i] == ';' || str[i] == '|' || str[i] == '<' || str[i] == '>') 
			token_addback(&tokens, token_new(token_type_get(str, &i), NULL));
		else if (ft_isspace(str[i]))
			continue ;
		else
		{
			j = i;
			while (!is_operator(str[i + 1]) && str[i + 1])
				i++;
			token_addback(&tokens, token_new(WORD, ft_substr(str, j, i - j + 1)));
		}
	}
	return (tokens);
}
