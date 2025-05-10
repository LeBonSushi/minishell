/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 19:50:56 by jd-halle          #+#    #+#             */
/*   Updated: 2025/05/10 06:41:43 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void init_readline(void)
{
	rl_catch_signals = 0;
	setup_signals();
}

void handle_sigint(int sig)
{
	(void)sig;
	
	if (g_zebi.in_here)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_zebi.stop_here = 1;
		g_zebi.g_exit = 130;
		return;
	}
	
	// Si nous sommes en mode interactif (prompt)
	if (!g_zebi.g_in_cmd_execution)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else
	{
		write(STDOUT_FILENO, "\n", 1);
	}
	g_zebi.g_exit = 130;
}



void	handle_sigquit(int sig)
{
	(void)sig;

	if (g_zebi.g_in_cmd_execution)
	{
		write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
		g_zebi.g_exit = 131;
	}
}

// int exit_code(int code_to_set, bool should_set)
// {
// 	static int code = 0;
// 	if (should_set)
// 	{
// 		code = code_to_set;
// 	}
// 	return code;
// }

void	setup_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;
	
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_handler = &handle_sigint;
	sa_int.sa_flags = 0;
	
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_handler = &handle_sigquit;
	
	sa_quit.sa_flags = 0;
	
	sigaction(SIGINT, &sa_int, NULL);
	sigaction(SIGQUIT, &sa_quit, NULL);
}


