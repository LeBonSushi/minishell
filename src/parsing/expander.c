/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jd-halle <jd-halle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 04:28:23 by macorso           #+#    #+#             */
/*   Updated: 2025/05/15 09:22:05 by jd-halle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*join_token_values(t_token *first, t_token *second)
{
	char	*joined;
	char	*temp;

	temp = ft_strdup(first->value);
	joined = ft_strjoin(temp, second->value, 1, 1);
	return (joined);
}

t_token	*join_tokens(t_token *tmp, t_token *next, t_token **next_ptr)
{
	t_token	*new;
	char	*joined_value;

	joined_value = join_token_values(tmp, next);
	new = token_new(tmp->token_type, joined_value,
			tmp->single_quoted, tmp->double_quoted);
	new->space_after = next->space_after;
	*next_ptr = next->next;
	return (new);
}

char	*get_env_value(t_all *all, char *var_name)
{
	t_env	*env;

	env = all->env;
	while (env)
	{
		if (!ft_strcmp(env->key, var_name))
			return (ft_strdup(env->value));
		env = env->next;
	}
	return (ft_strdup(""));
}

char	*expand_status(t_all *all, int *i)
{
	(void)all;
	(*i) += 2;
	return (ft_itoa(g_zebi.g_exit));
}
