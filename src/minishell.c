/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 13:30:19 by macorso           #+#    #+#             */
/*   Updated: 2025/01/24 17:07:01 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// void	init_all(t_all *all, int ac, char **av, char **envp)
// {
// 	(void)ac;
// 	(void)av;
// }

int	main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	// t_all all;
	// t_env	env;

	// init_all(&all, ac, av, envp);
	for (int i = 0; envp[i]; i++)
	{
		char *value = find_value(envp[i]);
		char *key = find_key(envp[i]);
		printf("env: %s\nenv_key: %s\nenv_value: %s\n", envp[i], key, value);
	}
	
	return (0);
}