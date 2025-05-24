/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_parsing.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 13:31:35 by macorso           #+#    #+#             */
/*   Updated: 2025/05/24 16:24:23 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	check_token_errors(t_token *curr)
{
	if (is_redir(curr) && is_redir(curr->next))
	{
		ft_fprintf(2, "ministress: syntax error near "
		"unexpected token `%s'\n", curr->next->value);
		return (true);
	}
	return (false);
}

bool	check_invalids(t_token *tokens)
{
	t_token	*curr;
	bool	error;

	if (!tokens)
		return (true);
	if (tokens->token_type == PIPE)
	{
		free_all_token(tokens);
		ft_fprintf(2, "ministress: syntax error near unexpected token `|'\n");
		return (true);
	}
	curr = tokens;
	while (curr->next)
	{
		error = check_token_errors(curr);
		if (error == true)
			return (free_all_token(tokens), true);
		curr = curr->next;
	}
	if (is_redir(curr))
		return(free_all_token(tokens), 
		ft_putstr_fd("syntax error near unexpected "
		"token `newline'\n", 2), 1);
	return (false);
}

static int	check_quote_continuation(char *line)
{
	int		i;
	int		in_single;
	int		in_double;

	i = 0;
	in_single = 0;
	in_double = 0;
	while (line[i])
	{
		if (line[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (line[i] == '"' && !in_single)
			in_double = !in_double;
		i++;
	}
	return (in_single || in_double);
}

static int	is_redirection_before_pipe(char *line)
{
	int		i;
	int		in_single;
	int		in_double;

	i = 0;
	in_single = 0;
	in_double = 0;
	while (line[i])
	{
		if (line[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (line[i] == '"' && !in_single)
			in_double = !in_double;
		else if (!in_single && !in_double)
		{
			if ((line[i] == '>' || line[i] == '<') && 
				line[i + 1] && line[i + 1] == '|')
				return (1);
			if (line[i] == '>' && line[i + 1] && line[i + 1] == '>' &&
				line[i + 2] && line[i + 2] == '|')
				return (1);
		}
		i++;
	}
	return (0);
}

static int	has_incomplete_redirection(char *line)
{
	int	i;
	int	len;

	len = ft_strlen(line);
	if (len < 2)
		return (0);
	i = len - 1;
	while (i >= 0 && ft_isspace(line[i]))
		i--;
	if (i >= 0 && line[i] == '|')
	{
		i--;
		while (i >= 0 && ft_isspace(line[i]))
			i--;
		if (i >= 1 && line[i] == '>' && line[i - 1] == '>')
			return (1);
		if (i >= 0 && (line[i] == '>' || line[i] == '<'))
			return (1);
	}
	return (0);
}

static int	check_pipe_continuation(char *line)
{
	int	i;

	if (check_quote_continuation(line))
		return (0);
	if (is_redirection_before_pipe(line))
		return (0);
	if (has_incomplete_redirection(line))
		return (0);
	i = ft_strlen(line) - 1;
	while (i >= 0 && ft_isspace(line[i]))
		i--;
	if (i >= 0 && line[i] == '|')
		return (1);
	return (0);
}


static char	*get_continuation_prompt(char *line)
{
	if (check_quote_continuation(line))
		return ("quote> ");
	if (check_pipe_continuation(line))
		return ("pipe> ");
	return (NULL);
}

static char	*join_lines(char *old_line, char *new_line)
{
	char	*result;

	if (!old_line || !new_line)
		return (NULL);
	result = ft_strjoin(old_line, "\n", 1, 0);
	if (!result)
		return (NULL);
	result = ft_strjoin(result, new_line, 1, 1);
	return (result);
}

char	*handle_line_continuation(char *line)
{
	char	*continuation_line;
	char	*full_line;
	char	*prompt;

	if (!line)
		return (NULL);
	full_line = ft_strdup(line);
	if (!full_line)
		return (NULL);
	while (1)
	{
		prompt = get_continuation_prompt(full_line);
		if (!prompt)
			break ;
		continuation_line = readline(prompt);
		if (!continuation_line)
		{
			free(full_line);
			return (NULL);
		}
		full_line = join_lines(full_line, continuation_line);
	}
	return (full_line);
}
