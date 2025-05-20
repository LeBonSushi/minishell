/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:57:02 by jd-halle          #+#    #+#             */
/*   Updated: 2025/05/20 20:15:34 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_dollar_sign(t_all *all, char *word, int *i, char **new_word)
{
	if (word[*i + 1] == '?')
		*new_word = ft_strjoin(*new_word, expand_status(all, i), 1, 1);
	else if (!ft_isalpha(word[*i + 1]) && !ft_isdigit(word[*i + 1])
		&& word[*i + 1] != '_')
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
	new->expanded = false;
	free(value);
	new->next = NULL;
	return (new);
}

t_token	*handle_token_expansion(t_all *all, t_token *tmp)
{
	t_token	*new;
	char	*expanded_value;

	if (tmp->token_type == WORD && !tmp->single_quoted)
	{
		expanded_value = expand_word(all, tmp->value);
		new = token_new4(tmp->token_type, expanded_value,
				tmp->single_quoted, tmp->double_quoted);
		new->expanded = true;
	}
	else if (tmp->token_type == TILDE && !tmp->single_quoted)
	{
		expanded_value = get_value(all->env, "HOME");
		if (!expanded_value)
			return (NULL);
		new = token_new(WORD, expanded_value,
				tmp->single_quoted, tmp->double_quoted);
		free(expanded_value);
		new->expanded = true;
	}
	else
	{
		new = token_new4(tmp->token_type, ft_strdup(tmp->value),
				tmp->single_quoted, tmp->double_quoted);
	}
	return (new);
}
