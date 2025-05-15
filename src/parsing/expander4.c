/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jd-halle <jd-halle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:57:19 by jd-halle          #+#    #+#             */
/*   Updated: 2025/05/15 10:44:02 by jd-halle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_isspecial(char c)
{
	if (c == '+' || c == '[' || c == ']' || c == '_' || c == '*'
		|| c == '#' || c == '@')
		return (true);
	return (false);
}

char	*ft_strjoinchar(char *s1, const char c)
{
	int		len;
	char	*str;

	if (!s1)
	{
		str = malloc(2);
		if (!str)
			return (NULL);
		str[0] = c;
		str[1] = '\0';
		return (str);
	}
	len = ft_strlen(s1);
	str = (char *)malloc(sizeof(char) * (len + 2));
	if (!str)
	{
		free(s1);
		return (NULL);
	}
	ft_strlcpy(str, s1, len + 1);
	str[len] = c;
	str[len + 1] = '\0';
	free(s1);
	return (str);
}

void	skip_variable_chars(char *word, int *i)
{
	while (word[*i] && (ft_isalnum(word[*i]) || word[*i] == '_'))
	{
		(*i)++;
	}
}

char	*expand_variable(t_all *all, char *word, int *i)
{
	int		start;
	char	*var_name;
	char	*value;

	(*i)++;
	start = *i;
	if (ft_isdigit(word[*i]))
		(*i)++;
	else
		skip_variable_chars(word, i);
	var_name = ft_substr(word, start, *i - start);
	value = get_env_value(all, var_name);
	free(var_name);
	return (value);
}

void	handle_variable_expansion(t_all *all, char *word,
	int *i, char **new_word)
{
	char	*expansion;

	expansion = expand_variable(all, word, i);
	*new_word = ft_strjoin(*new_word, expansion, 1, 1);
}
