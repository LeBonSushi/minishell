/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dfeve <dfeve@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 22:59:08 by dfeve             #+#    #+#             */
/*   Updated: 2025/01/28 00:28:02 by dfeve            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	split_get_size(char **split)
{
	int	i;

	i = 0;
	while (split && split[i])
		i++;
	return (i);
}

static t_token	get_token(char *str)
{
	t_token	token;

	if (ft_strcmp(str, "|") == 0)
		token = PIPE;
	else if (ft_strcmp(str, ";") == 0)
		token = SEMICOLON;
	else if (ft_strcmp(str, "<") == 0)
		token = REDIR_IN;
	else if (ft_strcmp(str, ">") == 0)
		token = REDIR_OUT;
	else if (ft_strcmp(str, ">>") == 0)
		token = REDIR_APPEND;
	else if (ft_strcmp(str, "<<") == 0)
		token = REDIR_HEREDOC;
	else
		token = WORD;
	return (token);
}

void	print_token(t_token	*token)
{
	int	i;

	i = 0;
	if (!token)
		return ;
	while (token[i])
		printf("%d\n", token[i++]);
}

t_command	*parse_str_to_command(char *str)
{
	t_command	*result;
	t_token		*token;
	char		**split;
	int			i;

	result = malloc(sizeof(t_command));
	if (!result)
		return (NULL);
	split = split_2_sep(str, ' ', "<");
	if (!split)
		return (NULL);
	i = -1;
	token = malloc((split_get_size(split) + 1)* sizeof(t_token));
	while (split[++i])
		token[i] = get_token(split[i]);
	token[i] = 0;
	result->args = split;
	result->token = token;
	return (result);
}