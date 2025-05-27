/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_parsing.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 13:31:35 by macorso           #+#    #+#             */
/*   Updated: 2025/05/27 16:45:33 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_error_pipe(t_token *tokens)
{
	if (tokens->next && tokens->next->token_type == PIPE)
		ft_fprintf(2, "ministress: syntax error near unexpected token `||'\n");
	else
		ft_fprintf(2, "ministress: syntax error near unexpected token `|'\n");
}

bool	check_token_errors(t_token *curr)
{
	if ((is_redir(curr) && is_redir(curr->next))
		|| (curr->token_type == PIPE && curr->next->token_type == PIPE))
	{
		ft_fprintf(2, "ministress: syntax error near "
			"unexpected token `%s'\n", curr->next->value);
		return (true);
	}
	if (is_redir(curr) && curr->next->token_type == PIPE)
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
		return (false);
	if (tokens->token_type == PIPE)
	{
		print_error_pipe(tokens);
		return (true);
	}
	curr = tokens;
	while (curr->next)
	{
		error = check_token_errors(curr);
		if (error == true)
			return (true);
		curr = curr->next;
	}
	if (is_redir(curr))
		return (ft_putstr_fd("ministress: syntax error near unexpected "
				"token `newline'\n", 2), true);
	return (false);
}

char	*join_lines_nl(char *old_line, char *new_line)
{
	char	*result;

	if (!old_line || !new_line)
	{
		if (new_line)
			free(new_line);
		if (old_line)
			free(old_line);
		return (NULL);
	}
	result = ft_strjoin(old_line, "\n", 1, 0);
	if (!result)
		return (NULL);
	result = ft_strjoin(result, new_line, 1, 1);
	return (result);
}

void	print_error_continuation(char *input, t_all *all)
{
	char	*close;

	close = get_continuation_prompt(input);
	g_exit = 2;
	if (!ft_strcmp(close, "quote> "))
	{
		ft_fprintf(2, "ministress: unexpected EOF while looking for matching "
			"`%c'\n", get_unclosed_quote(input));
		ft_fprintf(2, "ministress: syntax error: unexpected end of file\n");
	}
	else if (!ft_strcmp(close, "pipe> "))
	{
		ft_fprintf(2, "ministress: syntax error: unexpected end of file\n");
		ft_exit(NULL, all, NULL);
	}
}
