/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jd-halle <jd-halle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 13:30:19 by macorso           #+#    #+#             */
/*   Updated: 2025/02/26 08:46:05 by jd-halle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	init_all(t_all *all, int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;

	all->env = fill_env(envp);
	if (!all->env)
		printf("Erreur: fill_env pour env a retourné NULL\n");
	all->env_no_sorted = fill_env(envp);
	if (!all->env_no_sorted)
		printf("Erreur: fill_env pour env_no_sorted a retourné NULL\n");
	all->status = 0;
	all->tokens = NULL;
	all->redir_in_names = NULL;
	all->redir_in_names = NULL;
}

bool	env_empty(t_all *all)
{
	t_env	*env;

	env = all->env;
	if (!env)
		return (true);
	return (false);
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

void minishell_loop(t_all *all, char **envp)
{
	char *line;
	(void)all;
	(void)envp;

	while(1)
	{
		line = readline("ministress$ ");
		if (!line || !*line)
			continue ;
		add_history(line);
		all->redir_in_names = NULL;
		all->redir_out_names = NULL;
		all->tokens = fill_tokens(line, all);
		all->tokens = create_expand_tokens(all, all->tokens);
		all->tokens = merge_tokens(all->tokens);
		//print_tokens(all->tokens);
		ft_redirection(all, all->tokens);           //???
		//print_tokens(all->tokens);
		execute_builtins(all->tokens, &all->env, all);
		free(line);
		free_all_token(all->tokens);
	}
}

int	main(int ac, char **av, char **envp)
{
	t_all all;
	// char	*keys[] = {NULL};

	init_all(&all, ac, av, envp);
	minishell_loop(&all, envp);
	free_all(&all);
	return (0);
}
