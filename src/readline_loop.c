/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_loop.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 23:36:56 by macorso           #+#    #+#             */
/*   Updated: 2025/05/26 23:38:00 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*read_line(t_all *all)
{
	char	*line;

	all->tokens = NULL;
	all->cmds = NULL;
	all->paths = NULL;
	set_interactive_mode();
	line = readline("ministress$ ");
	all->num_line++;
	return (line);
}

void	parse_exec(t_all *all)
{
	manage_ugly_cases(all->tokens);
	all->tokens = create_expand_tokens(all, all->tokens);
	all->tokens = merge_tokens(all->tokens);
	all->cmds = create_cmdlist(all->tokens);
	g_zebi.g_exit = manage_redirections(all, all->cmds);
	exec_cmds(all, all->cmds);
	free_all_token(all->tokens);
	free_all_cmds(all->cmds);
}

bool	fill_tok_and_check_errors(char *input, t_all *all, char *post_quote_i)
{
	add_history(input);
	free(input);
	all->tokens = fill_tokens(post_quote_i);
	free(post_quote_i);
	if (check_invalids(all->tokens))
	{
		free_all_token(all->tokens);
		g_zebi.g_exit = 2;
		return (true);
	}
	return (false);
}

void	minishell_loop(t_all *all)
{
	char	*input;
	char	*post_quote_i;

	while (1)
	{
		input = read_line(all);
		if (input == NULL)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break ;
		}
		post_quote_i = handle_line_continuation(input, all);
		if (!*input || !post_quote_i)
		{
			free(input);
			continue ;
		}
		if (fill_tok_and_check_errors(input, all, post_quote_i))
			continue ;
		parse_exec(all);
	}
	rl_clear_history();
}
