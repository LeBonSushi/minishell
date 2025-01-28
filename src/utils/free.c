/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 18:49:42 by macorso           #+#    #+#             */
/*   Updated: 2025/01/28 04:21:13 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_env(t_env *env)
{
	free(env->key);
	free(env->value);
	free(env);
}

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	if (!tab && !*tab)
		return ;
	while (tab[i])
		free(tab[i++]);
	free(tab);
}

void	free_command(t_command *com)
{
	free_tab(com->args);
	if (com->token)
		free(com->token);
}

void	free_all_env(t_env *env)
{
	t_env	*next;

	while (env)
	{
		next = env->next;
		free_env(env);
		env = next;
	}
}