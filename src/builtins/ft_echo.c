/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jd-halle <jd-halle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 18:32:46 by macorso           #+#    #+#             */
/*   Updated: 2025/04/29 23:02:02 by jd-halle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	is_only_n_and_(char *str)
{
	int i = 1;
	if (str[0] != '-' || !str[1])
		return (false);
	while (str[i])
	{
		if (str[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}
void	ft_echo(t_token *tokens)
{
	int		newline;
	t_token	*current;
	
	current = tokens->next;
	newline = 1;
	if (!tokens)
		return;
	while (current && current->value[0] == '-' && is_only_n_and_(current->value))
	{
		newline = 0;
		current = current->next;
	}
	while (current)
	{
		ft_putstr_fd(current->value, 1);
		if (current->next)
			ft_putstr_fd(" ", 1);
		current = current->next;
	}
	if (newline)
		printf("\n");
}
