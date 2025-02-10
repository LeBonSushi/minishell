/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 22:33:53 by macorso           #+#    #+#             */
/*   Updated: 2025/02/10 01:36:36 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	ft_match_pattern(char *str, char *pattern)
{
	if (*pattern == '*' && *(pattern + 1) == '\0')
		return (true);
	if (*pattern == '\0' && *str == '\0')
		return (true);
	if (*pattern == '*')
		return (ft_match_pattern(str, pattern + 1)
			|| (*str && ft_match_pattern(str + 1, pattern)));
	if (*pattern == *str)
		return (ft_match_pattern(str + 1, pattern + 1));
	return (false);
}