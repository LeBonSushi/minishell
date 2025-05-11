/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 01:38:28 by macorso           #+#    #+#             */
/*   Updated: 2025/05/11 02:56:31 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_exit(t_token *tokens, t_all *all)
{
	t_token *current;
	int i;
	int res;

	ft_putstr_fd("exit\n", 1);
	current = tokens->next;
	if (!current)
	{
		free_all2(all);
		exit(g_zebi.g_exit);
	}
	
	i = 0;
	while (current)
	{
		i++;
		current = current->next;
	}
	current = tokens->next;
	i = 0;
	if (current->value[0] == '-' || current->value[0] == '+')
		i = 1;
	{
		ft_fprintf(2, "ministress: exit : too many arguments\n");
		exit(1);
	}
	i = 1;
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
				g_zebi.g_exit = 2;
				exit(g_zebi.g_exit);
			}
			i++;
		}
		printf("exit\n");
		res = ft_atoi(current->value) % 256;
		printf("res = %d\n", res);
		g_zebi.g_exit = res;
		exit(g_zebi.g_exit);
	}
	else
	{
		printf("exit\n");
		g_zebi.g_exit = 0;
		// printf("code de sortie: %d\n", g_zebi.g_exit);
		exit (g_zebi.g_exit);
	}
	free_all2(all);
	all->cmds = NULL;
	all->tokens = NULL;
}
