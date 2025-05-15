/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jd-halle <jd-halle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 09:47:18 by jd-halle          #+#    #+#             */
/*   Updated: 2025/05/15 09:47:27 by jd-halle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	get_unclose_quote(char *str)
{
	int		i;
	bool	sq;
	bool	dq;

	i = 0;
	sq = false;
	dq = false;
	while (str[i])
	{
		if (str[i] == '\'' && !dq)
			sq = !sq;
		if (str[i] == '\"' && !sq)
			dq = !dq;
		i++;
	}
	if (dq)
		return ('"');
	return ('\'');
}

char	*manage_unclosed_quote(char *str, char quote)
{
	char	*line;
	char	*result;
	char	*line_with_nl;

	result = str;
	result = ft_strjoin(result, "\n", 0, 0);
	if (!result)
		return (NULL);
	while (1)
	{
		if (quote == '"')
			line = readline("dquote> ");
		else
			line = readline("quote> ");
		if (!line)
			break ;
		line_with_nl = ft_strjoin(line, "\n", 1, 0);
		result = ft_strjoin(result, line_with_nl, 1, 1);
		if (!result)
			return (NULL);
		if (is_quotes_closed(result))
			break ;
	}
	return (result);
}

t_token	*fill_tokens(char *str)
{
	t_token	*tokens;
	char	*modified_str;
	char	*tmp;

	if (!str)
		return (NULL);
	if (!is_quotes_closed(str))
		modified_str = manage_unclosed_quote(str, get_unclose_quote(str));
	else
		modified_str = ft_strdup(str);
	if (!modified_str)
		return (NULL);
	if (*modified_str)
		add_history(modified_str);
	tokens = NULL;
	tmp = modified_str;
	while (*modified_str)
		token_addback(&tokens, new_parsed_token(&modified_str));
	free(tmp);
	return (tokens);
}
