/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jd-halle <jd-halle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 18:32:46 by macorso           #+#    #+#             */
/*   Updated: 2025/02/22 02:28:59 by jd-halle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void clear_terminal(void)
{
    write(1, "\033[H\033[J", 7);
}
void	ft_echo(t_token *tokens)
{
	int newline;
	t_token *current = tokens->next;
	
	if (!tokens)
		return;
	newline = 1;
	if (current && ft_strcmp(current->value, "-n") == 0)
	{
		newline = 0;
		current = current->next;
	}
	while (current)
	{
		if (ft_strcmp(current->value, "-n") != 0)
		{
			ft_putstr_fd(current->value, 1);
			if (current->next)
				ft_putstr_fd(" ", 1);
		}
		current = current->next;
	}
	if (newline)
		ft_printf("\n");
}
