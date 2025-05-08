/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jd-halle <jd-halle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 22:30:10 by jd-halle          #+#    #+#             */
/*   Updated: 2025/05/08 01:03:05 by jd-halle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
int g_zebi.g_exit;

void	minishell_loop(t_all *all)
{
	char	*line;

	while (1)
	{
		all->tokens = NULL;
		all->cmds = NULL;
		all->paths = NULL;
		line = readline("ministress$ ");
		if (line == NULL)
		{
			write(STDOUT_FILENO, "\n", 1);
			break ;
		}
		if (*line)
			add_history(line);
		all->tokens = fill_tokens(line);
		all->tokens = create_expand_tokens(all, all->tokens);
		all->tokens = merge_tokens(all->tokens);
		all->cmds = create_cmdlist(all->tokens);
		g_zebi.g_exit = manage_redirections(all, all->cmds);
		exec_cmds(all, all->cmds);
		free(line);
		free_all_token(all->tokens);
		free_all_cmds(all->cmds);
	}
}

int	main(int ac, char **av, char **envp)
{
	t_all all;
	// char	*keys[] = {NULL};

	init_readline(&all.status);
	init_all(&all, ac, av, envp);
	minishell_loop(&all);
	free_all(&all);
	return (0);
}
