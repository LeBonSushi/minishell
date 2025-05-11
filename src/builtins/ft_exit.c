/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jd-halle <jd-halle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 01:38:28 by macorso           #+#    #+#             */
/*   Updated: 2025/05/11 03:30:02 by jd-halle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_exit(t_token *tokens, t_all *all)
{
	t_token	*current;
	int		i;
	int		res;

	ft_fprintf(1, "exit\n");
	current = tokens->next;
	if (!current)
	{
		free_all(all);
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
	while (current->value[i])
	{
		if (!ft_isdigit(current->value[i]))
		{
			ft_fprintf(2, "ministress: exit: %s: numeric argument required\n", current->value);
			free_all(all);
			exit(2);
		}
		i++;
	}
	if (tokens->next->next)
	{
		ft_fprintf(2, "ministress: exit: too many arguments\n");
		g_zebi.g_exit = 1;
		return;
	}
	res = ft_atoi(current->value) % 256;
	free_all(all);
	g_zebi.g_exit = res;
	exit (g_zebi.g_exit);
}
