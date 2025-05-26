/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 13:30:19 by macorso           #+#    #+#             */
/*   Updated: 2025/05/26 14:15:44 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <termios.h>

t_zebi	g_zebi;

void	minishell_loop(t_all *all)
{
	char	*line;
	char	*post_quote_i;

	while (1)
	{
		all->tokens = NULL;
		all->cmds = NULL;
		all->paths = NULL;
		set_interactive_mode();
		line = readline("ministress$ ");
		all->num_line++;
		if (line == NULL)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break ;
		}
		if (*line)
			add_history(line);
		post_quote_i = handle_line_continuation(line, all);
		free(line);
		if (!post_quote_i)
			continue ;
		all->tokens = fill_tokens(post_quote_i);
		free(post_quote_i);
		if (check_invalids(all->tokens))
		{
			free_all_token(all->tokens);
			g_zebi.g_exit = 2;
			continue ;
		}
		manage_ugly_cases(all->tokens);
		all->tokens = create_expand_tokens(all, all->tokens);
		all->tokens = merge_tokens(all->tokens);
		if (!is_valid_tokens(all->tokens))
		{
			free_all_token(all->tokens);
			continue ;
		}
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
	minishell_loop(&all);
	free_all(&all);
	return (g_zebi.g_exit);
}
