/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jd-halle <jd-halle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 13:30:19 by macorso           #+#    #+#             */
/*   Updated: 2025/03/01 22:20:49 by jd-halle         ###   ########.fr       */
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

static char	**tokens_to_array(t_token *tokens)
{
	t_token	*curr;
	char	**args;
	int		count;
	int		i;

	count = 0;
	curr = tokens;
	while (curr)
	{
		if (curr->token_type == WORD)
			count++;
		curr = curr->next;
	}
	args = malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	i = 0;
	curr = tokens;
	while (curr)
	{
		if (curr->token_type == WORD)
			args[i++] = ft_strdup(curr->value);
		curr = curr->next;
	}
	args[i] = NULL;
	return (args);
}

static char	*find_command_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	char	*full_path;
	int		i;

	while (*envp && ft_strncmp(*envp, "PATH=", 5))
		envp++;
	if (!*envp)
		return (ft_strdup(cmd));
	paths = ft_split(*envp + 5, ':');
	i = 0;
	while (paths[i])
	{
		path = ft_strjoin(paths[i], "/", 0, 0);
		full_path = ft_strjoin(path, cmd, 1, 0);
		if (access(full_path, X_OK) == 0)
		{
			free_tab(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_tab(paths);
	return (ft_strdup(cmd));
}

static void	execute_single_command(t_cmd *cmd, char **envp)
{
	char	**args;
	char	*path;

	if (cmd->skip_cmd)
		return ;
	args = tokens_to_array(cmd->tokens);
	if (!args || !args[0])
	{
		free_tab(args);
		exit(1);
	}
	path = find_command_path(args[0], envp);
	if (cmd->infile_fd != 0)
		dup2(cmd->infile_fd, STDIN_FILENO);
	if (cmd->outfile_fd != 1)
		dup2(cmd->outfile_fd, STDOUT_FILENO);
	execve(path, args, envp);
	ft_putstr_fd("command not found: ", 2);
	ft_putendl_fd(args[0], 2);
	free_tab(args);
	free(path);
	exit(127);
}

void	execute_pipeline(t_cmd *cmds, char **envp)
{
	t_cmd	*curr;
	int		pipe_fd[2];
	pid_t	pid;
	int		prev_pipe;

	curr = cmds;
	prev_pipe = 0;
	while (curr)
	{
		if (curr->next && pipe(pipe_fd) == -1)
			return ;
		pid = fork();
		if (pid == -1)
			return ;
		if (pid == 0)
		{
			if (prev_pipe)
			{
				dup2(prev_pipe, STDIN_FILENO);
				close(prev_pipe);
			}
			if (curr->next)
			{
				close(pipe_fd[0]);
				if (curr->outfile_fd == 1)
					dup2(pipe_fd[1], STDOUT_FILENO);
				close(pipe_fd[1]);
			}
			execute_single_command(curr, envp);
		}
		if (prev_pipe)
			close(prev_pipe);
		if (curr->next)
		{
			close(pipe_fd[1]);
			prev_pipe = pipe_fd[0];
		}
		curr->pid = pid;
		curr = curr->next;
	}
	curr = cmds;
	while (curr)
	{
		waitpid(curr->pid, NULL, 0);
		curr = curr->next;
	}
}

void minishell_loop(t_all *all, char **envp)
{
	char	*line;
	char	**env;
	(void)envp;
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
		all->cmds = create_cmdlist(all->tokens);
		env = env_to_tab(all->env);
		// debug_print_cmdlist(all->cmds);
		//print_tokens(all->tokens);
		// ft_redirection(all, all->tokens);           //???
		//print_tokens(all->tokens);
		manage_redirections(all->cmds);
		// if (is_builtin(all->tokens))
			// execute_builtins(all->tokens, &all->env, all);
		// else
			execute_pipeline(all->cmds, env);
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
