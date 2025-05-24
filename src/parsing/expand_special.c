/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_special.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 16:41:11 by macorso           #+#    #+#             */
/*   Updated: 2025/05/24 16:55:21 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	manage_ugly_cases(t_token *tokens)
{
	t_token	*curr;
	char	*new_value;

	curr = tokens;
	while (curr && curr->next)
	{
		if (ft_strlen(curr->value) >= 1 && curr->value[ft_strlen(curr->value) - 1] == '$'
			&& curr->next->double_quoted)
		{
			new_value = ft_substr(curr->value, 0, ft_strlen(curr->value) - 1);
			free(curr->value);
			curr->value = new_value;
			curr->next->double_quoted = false;
			if (ft_strlen(curr->value) == 0)
			{
				delete_token(&tokens, curr);
				curr = tokens;
				continue ;
			}
		}
		curr = curr->next;
	}
}
