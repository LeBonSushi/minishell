/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 13:30:19 by macorso           #+#    #+#             */
/*   Updated: 2025/05/15 01:51:49 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
t_zebi g_zebi;

void	init_all(t_all *all, int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	all->env = fill_env(envp);
	all->tokens = NULL;
	all->cmds = NULL;
	all->paths = NULL;
	g_zebi.g_exit = 0;
}

bool	env_empty(t_all *all)
{
	t_env	*env;

	env = all->env;
	if (!env)
		return (true);
	return (false);
}

char	*join_parsed_tokens(t_token *tokens)
{
	t_token	*curr;
	char	*result;
	char	*tmp;

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
	t_token	*curr;
	t_token	*prev;
	t_token	*next;

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

void	free_all_cmds(t_cmd *cmds)
{
	t_cmd	*curr;
	t_cmd	*next;

	if (!cmds)
		return ;
	curr = cmds;
	while (curr)
	{
		next = curr->next;
		if (curr->path)
			free(curr->path);
		if (curr->tokens)
			free_all_token(curr->tokens);
		free(curr);
		curr = next;
	}
	
}

void	minishell_loop(t_all *all)
{
	char	*line;

	while (1)
	{
		all->tokens = NULL;
		all->cmds = NULL;
		all->paths = NULL;
		set_interactive_mode();
		line = readline("ministress$ ");
		if (line == NULL)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break ;
		}
		all->tokens = fill_tokens(line);
		free(line);
		if (!all->tokens)
			continue ;
		all->tokens = create_expand_tokens(all, all->tokens);
		// print_tokens(all->tokens);
		if (!all->tokens)
			continue ;
		all->tokens = merge_tokens(all->tokens);
		if (!all->tokens)
			continue ;
		// test = split_tokens(all->tokens);
		all->cmds = create_cmdlist(all->tokens);
		if (!all->cmds)
		{
			free_all_token(all->tokens);
			continue ;
		}
		g_zebi.g_exit = manage_redirections(all, all->cmds);
		// debug_print_cmdlist(all->cmds);
		exec_cmds(all, all->cmds);
		free_all_token(all->tokens);
		free_all_cmds2(all->cmds);
	}
	rl_clear_history();
}

int	main(int ac, char **av, char **envp)
{
	t_all all;
	// char	*keys[] = {NULL};

	init_readline();
	init_all(&all, ac, av, envp);
	minishell_loop(&all);
	free_all(&all);
	return (g_zebi.g_exit);
}
