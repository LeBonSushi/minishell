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

#include "minishell.h"

void	print_sorted_env(t_env *env)
{
	t_env	*sorted_env;

	sorted_env = env_create_sorted(env);
	if (sorted_env)
	{
		print_env(sorted_env);
		free_all_env(sorted_env);
	}
}
