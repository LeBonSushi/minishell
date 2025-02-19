/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 13:30:19 by macorso           #+#    #+#             */
/*   Updated: 2025/02/19 05:26:54 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// void	init_all(t_all *all, int ac, char **av, char **envp)
// {
// 	(void)ac;
// 	(void)av;
// }

void	init_all(t_all *all, int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;

	all->env = fill_env(envp);
	all->status = 0;
	all->tokens = NULL;
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
	char	*pwd_key = ft_strjoin("PWD=", pwd, 0, 1);
	char	*keys[] = {"OLDPWD=", pwd_key,
		"PROMPT_COMMAND=RETRN_VAL=$?;logger -p local6.debug\
		\"$(history 1 | sed \"s/^[ ]*[0-9]\\+[ ]*//\" )\
		[$RETRN_VAL]\"", "SHLVL=1", NULL};
	if (env_empty(all))
		export_builtin(all, keys);
}

char *join_parsed_tokens(t_token *tokens)
{
    t_token *curr;
    char *result;
    char *tmp;

    if (!tokens)
        return (ft_strdup(""));
    
    result = ft_strdup(tokens->value);
    if (!result)
        return (NULL);
    
    curr = tokens->next;
    while (curr)
    {
        if (curr->space_after)
        {
            tmp = ft_strjoin(result, " ", 1, 0);
            if (!tmp)
                return (NULL);
            result = tmp;
        }
        tmp = ft_strjoin(result, curr->value, 1, 0);
        if (!tmp)
            return (NULL);
        result = tmp;
        curr = curr->next;
    }
    return (result);
}

t_token	*delete_empty_tokens(t_token *tokens)
{
	t_token *curr;
	t_token *prev;
	t_token *next;

	curr = tokens;
	prev = NULL;
	while (curr)
	{
		next = curr->next;
		if (curr->value && !*curr->value && !curr->space_after)
			delete_token(&tokens, curr);
		else
			prev = curr;
		curr = next;
	}
	return (tokens);
}

void minishell_loop(t_all *all)
{
    char *line;
	(void)all;

	while(1)
	{
		line = readline("ministress$ ");
		if (!line || !*line)
			continue ;
		add_history(line);
		all->tokens = fill_tokens(line, all);
		all->tokens = create_expand_tokens(all, all->tokens);
		all->tokens = merge_tokens(all->tokens);
		print_tokens(all->tokens);
		free(line);
		free_all_token(all->tokens);
	}
}

int	main(int ac, char **av, char **envp)
{
	t_all all;
	// char	*keys[] = {NULL};

	init_all(&all, ac, av, envp);
	minishell_loop(&all);
	free_all(&all);
	return (0);
}