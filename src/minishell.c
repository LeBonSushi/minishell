/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 13:30:19 by macorso           #+#    #+#             */
/*   Updated: 2025/02/03 15:40:29 by macorso          ###   ########.fr       */
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

bool	env_empty(t_all *all)
{
	t_env	*env;

	env = all->env;
	if (!env)
		return (true);
	return (false);
}

void	verify_env(t_all *all)
{
	char	*pwd = pwd_builtin_char();
	char	*pwd_key = ft_strjoin("PWD=", pwd);
	free(pwd);
	char	*keys[] = {"OLDPWD=", pwd_key,
		"PROMPT_COMMAND=RETRN_VAL=$?;logger -p local6.debug\
		\"$(history 1 | sed \"s/^[ ]*[0-9]\\+[ ]*//\" )\
		[$RETRN_VAL]\"", "SHLVL=1", NULL};
	if (env_empty(all))
		export_builtin(all, keys);
}

int	main(int ac, char **av, char **envp)
{
	t_all all;
	// char	*keys[] = {NULL};

	init_all(&all, ac, av, envp);
	// verify_env(&all);
	// printf("\n\n\n\n");
	// export_builtin(&all, NULL);
	// print_header();
	// env_builtin(&all);
	// env_builtin(&all);
	minishell_loop(&all);
	free_all(&all);
	return (0);
}