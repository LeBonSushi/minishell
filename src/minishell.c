/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 13:30:19 by macorso           #+#    #+#             */
/*   Updated: 2025/05/26 23:36:54 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <termios.h>

t_zebi	g_zebi;

int	main(int ac, char **av, char **envp)
{
	t_all	all;

	init_readline();
	init_all(&all, ac, av, envp);
	minishell_loop(&all);
	free_all(&all);
	return (g_zebi.g_exit);
}
