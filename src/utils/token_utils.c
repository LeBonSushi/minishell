/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 16:00:14 by macorso           #+#    #+#             */
/*   Updated: 2025/02/03 17:40:47 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	is_operator(char c)
{
	return (c == ';' || c == '|' || c == '<' || c == '>' || ft_isspace(c));
}

void	increment_str(char **str)
{
	(*str)++;
}

t_token_type	get_token_type(char *str)
{
	if (*str == ';')
		return (increment_str(&str), SEMICOLON);
	else if (*str == '|')
		return (increment_str(&str), PIPE);
	else if (*str == '<')
		return (increment_str(&str), REDIR_IN);
	else if (*str == '>')
	{
		if (*(str + 1) == '>')
		{
			str++;
			return (increment_str(&str), REDIR_APPEND);
		}
		return (increment_str(&str), REDIR_OUT);
	}
	else
	{
		printf("Unknown token type\n");
		return (0);
	}
}
