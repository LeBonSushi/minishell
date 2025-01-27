/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 13:30:19 by macorso           #+#    #+#             */
/*   Updated: 2025/01/26 17:09:55 by macorso          ###   ########.fr       */
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
	t_env	*tmp;
	t_env	*next;

	tmp = all->env;
	while (tmp)
	{
		next = tmp->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
		tmp = next;
	}
}

int	main(int ac, char **av, char **envp)
{
	t_all all;
	char *keys[] = {"PATH", NULL};

	init_all(&all, ac, av, envp);
	env_builtin(&all);
	unset_builtin(keys, &all);
	printf("After unset:\n");
	env_builtin(&all);
	free_all(&all);
	return (0);
}