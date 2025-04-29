/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jd-halle <jd-halle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:57:19 by jd-halle          #+#    #+#             */
/*   Updated: 2025/04/29 21:57:57 by jd-halle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_special_char(char *word, int *i, char **new_word)
{
	if (ft_isspecial(word[*i + 1]) && word[*i + 1] != '_' && word[*i + 2])
	{
		(*i) += 2;
		*new_word = ft_strjoinchar(*new_word, word[(*i)++]);
	}
	else
		*new_word = ft_strjoinchar(*new_word, word[(*i)++]);
}

void	handle_dollar_sign(t_all *all, char *word, int *i, char **new_word)
{
	if (word[*i + 1] == '?')
		*new_word = ft_strjoin(*new_word, expand_status(all, i), 1, 1);
    else if (!ft_isalpha(word[*i + 1]) && !ft_isdigit(word[*i + 1]) && word[*i + 1] != '_')
		*new_word = ft_strjoinchar(*new_word, word[(*i)++]);
    else
        handle_variable_expansion(all, word, i, new_word);
}

char	*expand_word(t_all *all, char *word)
{
	int		i;
	char	*new_word;

	i = 0;
	new_word = ft_strdup("");
	while (word[i])
	{
		if (word[i] == '$' && word[i + 1])
			handle_dollar_sign(all, word, &i, &new_word);
		else
			new_word = ft_strjoinchar(new_word, word[i++]);
	}
	return (new_word);
}

char	*get_value(t_env *env, char *key)
{
	while (env)
	{
		if (!ft_strcmp(env->key, key))
			return (ft_strdup(env->value));
		env = env->next;
	}
	return (ft_strdup(""));
}

t_token	*token_new4(t_token_type token_type, char *value,
			bool single_q, bool double_q)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->token_type = token_type;
	new->value = ft_strdup(value);
	new->single_quoted = single_q;
	new->double_quoted = double_q;
	new->space_after = true;
	free(value);
	new->next = NULL;
	return (new);
}
