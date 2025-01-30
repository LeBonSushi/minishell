/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 13:30:19 by macorso           #+#    #+#             */
/*   Updated: 2025/01/30 01:51:06 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// void	init_all(t_all *all, int ac, char **av, char **envp)
// {
// 	(void)ac;
// 	(void)av;
// }

void	minishell_loop(t_all *all)
{
	env_builtin(all);
	char *line;

	while(1)
	{
		line = readline("minishell$ ");
		if (!line || !*line)
			continue ;
		add_history(line);
		all->tokens = fill_tokens(line);
		print_tokens(all->tokens);
		free(line);
		free_all_token(all->tokens);
	}
}
void	init_all(t_all *all, int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;

	all->env = fill_env(envp);
}

int	main(int ac, char **av, char **envp)
{
	t_all all;
	char	*keys[] = {NULL};

	init_all(&all, ac, av, envp);
	
	// env_builtin(&all);
	// printf("\n\n\n\n");
	export_builtin(&all, keys);
	// env_builtin(&all);
	minishell_loop(&all);
	free_all(&all);
	return (0);
}