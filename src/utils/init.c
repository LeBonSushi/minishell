/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 04:25:37 by jd-halle          #+#    #+#             */
/*   Updated: 2025/05/27 01:23:41 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_all(t_all *all, int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	all->env = fill_env(envp);
	all->tokens = NULL;
	all->cmds = NULL;
	all->paths = NULL;
	all->num_line = 0;
	g_exit = 0;
}
