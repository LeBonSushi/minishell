/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 18:15:44 by macorso           #+#    #+#             */
/*   Updated: 2025/03/04 23:06:46 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_env(t_env *env)
{
	t_env	*current;
	
	current = env;
	while (current)
	{
		printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
}
