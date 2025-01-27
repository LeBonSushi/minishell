/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 13:30:19 by macorso           #+#    #+#             */
/*   Updated: 2025/01/27 18:49:58 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// void	init_all(t_all *all, int ac, char **av, char **envp)
// {
// 	(void)ac;
// 	(void)av;
// }

void	free_all(t_all *all)
{
	free_all_env(all);
}

int	main(int ac, char **av, char **envp)
{
	t_all all;
	char	*keys[] = {"key1=", "key2=value","=value3", NULL};

	init_all(&all, ac, av, envp);
	
	// env_builtin(&all);
	// printf("\n\n\n\n");
	export_builtin(&all, keys);
	// env_builtin(&all);
	free_all(&all);
	return (0);
}