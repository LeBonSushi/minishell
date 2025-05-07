/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:55:54 by jd-halle          #+#    #+#             */
/*   Updated: 2025/05/07 18:00:24 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*copy_token(t_token *tmp, t_token **next_ptr)
{
	t_token	*new;

	new = token_new(tmp->token_type, ft_strdup(tmp->value),
			tmp->single_quoted, tmp->double_quoted);
	new->space_after = tmp->space_after;
	*next_ptr = tmp->next;
	return (new);
}

t_token	*create_tokens_without_space(t_token *tokens)
{
	t_token	*new_tokens;
	t_token	*tmp;
	t_token	*next;
	t_token	*new;

	new_tokens = NULL;
	tmp = tokens;
	while (tmp)
	{
		next = tmp->next;
		if (next && is_same_type(tmp, next, WORD)
			&& is_same_quote(tmp, next) && !tmp->space_after)
			new = join_tokens(tmp, next, &next);
		else
		{
			new = token_new(tmp->token_type, tmp->value, tmp->single_quoted, tmp->double_quoted);
			tmp = next;
		}
		token_addback(&new_tokens, new);
	}
	free_all_token(tokens);
	return (new_tokens);
}

char *get_env_value(t_all *all, char *var_name)
{
	t_env *env;

	env = all->env;
	while (env)
	{
		if (!ft_strcmp(env->key, var_name))
			return (ft_strdup(env->value));
		env = env->next;
	}
	return (ft_strdup(""));  // Return empty string if not found
}

char *expand_status(t_all *all, int *i)
{
	(*i) += 2;  // Skip $?
	return (ft_itoa(g_exit));
}

char *expand_digit(int *i)
{
	(*i) += 2;  // Skip $n
	return (ft_strdup(""));  // $n expands to nothing in bash when n is a digit
}
