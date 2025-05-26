/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 01:38:28 by macorso           #+#    #+#             */
/*   Updated: 2025/05/26 11:20:54 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_token_size(t_token *token)
{
	t_token	*curr;
	int		i;

	curr = token;
	i = 0;
	while (curr)
	{
		i++;
		curr = curr->next;
	}
	return (i);
}

bool	str_isvalid(char *str, t_all *all)
{
	int	i;

	i = 0;
	if (!str)
		return (false);
	if (str[0] == '-' || str[0] == '+')
		i = 1;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
		{
			ft_fprintf(2, "ministress: exit: %s: \
numeric argument required\n", str);
			free_all(all);
			return (false);
		}
		i++;
	}
	return (true);
}

void	ft_exit(t_token *tokens, t_all *all, char **envp)
{
	t_token	*current;
	int		res;

	ft_fprintf(1, "exit\n");
	if (get_token_size(tokens) == 0 || get_token_size(tokens) == 1)
	{
		free_all(all);
		free_tab(envp);
		exit(g_zebi.g_exit);
	}
	current = tokens->next;
	if (!str_isvalid(current->value, all))
		return (free_tab(envp), exit(2));
	if (get_token_size(tokens) > 2)
	{
		ft_fprintf(2, "ministress: exit: too many arguments\n");
		g_zebi.g_exit = 1;
		return ;
	}
	res = ft_atoi(current->value) % 256;
	free_all(all);
	free_tab(envp);
	g_zebi.g_exit = res;
	exit(g_zebi.g_exit);
}

int	ft_exit2(t_all *all, int exit_code)
{
	(void)all;
	g_zebi.g_exit = exit_code;
	exit(exit_code);
}
