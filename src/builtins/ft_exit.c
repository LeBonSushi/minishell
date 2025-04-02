/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 01:38:28 by macorso           #+#    #+#             */
/*   Updated: 2025/04/01 11:44:08 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_exit(t_token *tokens, t_all *all)
{
	t_token *current;
	int i;
	int res;

	i = 0;
	current = tokens->next;
	if (current)
	{
		while (current->value[i])
		{
			if (!ft_isdigit(current->value[i]))
			{
				ft_putstr_fd("ministress: exit : ", 2);
				ft_putstr_fd(current->value, 2);
				ft_putstr_fd(": numeric argument required\n", 2);
				all->status = 2;
				//exit(all->status);
			}
			i++;
		}
		printf("exit\n");
		res = ft_atoi(current->value) % 256;
		all->status = res;
		printf("code de sortie: %d\n", all->status);
		exit(all->status);
	}
	else
	{
		printf("exit\n");
		all->status = 0;
		printf("code de sortie: %d\n", all->status);
		//exit (all->status);
	}
	free_all(all);
}
/*retirer les printf des code de sortie et les printf des exit  */