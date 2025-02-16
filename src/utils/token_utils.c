/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 16:00:14 by macorso           #+#    #+#             */
/*   Updated: 2025/02/17 00:09:01 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
 
bool	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}
bool	is_operator(char *str)
{
	if (*str == ';' || *str == '|' || *str == '<' || *str == '>' || *str == '~')
		return (true);
	if (*str == '&' && *(str + 1) == '&')
		return (true);
	return (false);
}
