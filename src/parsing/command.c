/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dfeve <dfeve@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 22:59:08 by dfeve             #+#    #+#             */
/*   Updated: 2025/01/28 03:35:40 by dfeve            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// static int	split_get_size(char **split)
// {
// 	int	i;

// 	i = 0;
// 	while (split && split[i])
// 		i++;
// 	return (i);
// }

t_token	get_token(char c)
{
	t_token	token;

	if (c == '|')
		token = PIPE;
	else if (c ==  ';')
		token = SEMICOLON;
	else if (c == '<')
		token = REDIR_IN;
	else if (c == '>')
		token = REDIR_OUT;
	else if (ft_isalnum(c) == 1)
		token = WORD;
	else
		token = -1;
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

int	token_str_get_size(char *token_str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (token_str && token_str[i])
	{
		if (i > 0)
			if ((token_str[i] != token_str[i - 1]) && token_str[i - 1] != '/')
				count++;
		i++;
	}
	if (token_str[i - 1] != '/')
		count++;
	return (count);
}

t_token	*parse_token(char *token_str)
{
	t_token	*result;
	int	i;
	int	y;

	i = 0;
	y = 0;
	result = malloc((token_str_get_size(token_str) + 1)* sizeof(t_token));
	while (token_str && token_str[i])
	{
		if (i > 0)
			if ((token_str[i] != token_str[i - 1]) && token_str[i - 1] != '/')
				result[y++] = token_str[i - 1] - '0';
		i++;
	}
	if (token_str[i - 1] != '/')
		result[y++] = token_str[i - 1] - '0';
	result[y] = 0;
	return (result);
}

t_command	*parse_str_to_command(char *str)
{
	t_command	*result;
	char		**split;
	char		*str_token;

	str_token = str_to_token(str);
	printf("token string: %s\n", str_token);
	result = malloc(sizeof(t_command));
	if (!result)
		return (NULL);
	split = split_str_token(str, str_token);
	if (!split)
		return (NULL);
	result->args = split;
	result->token = parse_token(str_token);
	//free(str_token);
	return (result);
}