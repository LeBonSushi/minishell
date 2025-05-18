/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 13:30:19 by macorso           #+#    #+#             */
/*   Updated: 2025/05/18 18:33:25 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <termios.h>
t_zebi	g_zebi;

void	minishell_loop(t_all *all)
{
	char	*line;
	while (1)
	{
		all->tokens = NULL;
		all->cmds = NULL;
		all->paths = NULL;
		set_interactive_mode();
		line = readline("ministress$ ");
		if (line == NULL)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break ;
		}
		all->tokens = fill_tokens(line);
		free(line);
		all->tokens = create_expand_tokens(all, all->tokens);
		all->tokens = merge_tokens(all->tokens);
		all->cmds = create_cmdlist(all->tokens);
		g_zebi.g_exit = manage_redirections(all, all->cmds);
		exec_cmds(all, all->cmds);
		free_all_token(all->tokens);
		free_all_cmds(all->cmds);
	}
	rl_clear_history();
}

int	main(int ac, char **av, char **envp)
{
	t_all	all;

	init_readline();
	init_all(&all, ac, av, envp);
	print_env(all.env);
	minishell_loop(&all);
	free_all(&all);
	return (g_zebi.g_exit);
}
