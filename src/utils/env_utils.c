/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 00:17:58 by macorso           #+#    #+#             */
/*   Updated: 2025/05/05 00:20:09 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char *find_value2(t_env *env, char *key)
{
	t_env *curr;

	curr = env;
	while (curr)
	{
		if (ft_strcmp(key, curr->key))
			return (curr->value);
		curr = curr->next;
	}
	return (NULL);
}