/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 13:30:19 by macorso           #+#    #+#             */
/*   Updated: 2025/02/17 23:20:08 by macorso          ###   ########.fr       */
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
	all->tree = NULL;
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

void	delete_char_from_str(char **str)
{
	char *tmp;
	int i;

	i = 0;
	tmp = ft_strdup(*str + 1);
	if (!tmp)
		return ;
	free(*str);
	*str = tmp;
}

char	*ft_parsing(char *line)
{
	int	i;
	int	count;
	int	start;
	pid_t	pid;
	
	if (!line)
		return (NULL);
	i = 0;
	count = 0;
	while (line[i])
	{
		while (ft_isspace(line[i]))
			i++;
		if (line[i] == '(')
		{
			if (count == 0)
				start = i;
			count++;
		}
		if (line[i] == ')')
		{
			count--;
			if (count == 0)
			{
				pid = fork();
				if (pid == -1)
					exit(1);
				if (pid == 0)
					ft_parsing(ft_substr(line, start + 1, i - start - 1));
				else
					waitpid(pid, NULL, 0);
			}
		}
		i++;
	}
	i = 0;
	while (line[i])
	{
		if (line[i] == '|' || line[i] == ')')
			delete_char_from_str(&line + i);
		i++;
	}
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
		all->tokens = expand_wildcard(all->tokens);
		all->tokens = merge_tokens(all->tokens);
		process_parenthesis(all->tokens, all);
		// print_tokens(all->tokens);
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