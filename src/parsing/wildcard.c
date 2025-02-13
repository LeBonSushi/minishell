/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 22:33:53 by macorso           #+#    #+#             */
/*   Updated: 2025/02/13 04:01:14 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool ft_match_pattern(char *str, char *pattern)
{
	if (!*pattern)
		return (!*str);
	if (*pattern == '*')
		return (ft_match_pattern(str, pattern + 1) || (*str && ft_match_pattern(str + 1, pattern)));
	if (*pattern == *str)
		return (ft_match_pattern(str + 1, pattern + 1));
	return (false);
}

static char	*merge_wildcard_values(t_token *wildcard_tokens)
{
	t_token	*curr;
	char	*merged;
	char	*tmp;

	if (!wildcard_tokens)
		return (ft_strdup("*"));
	merged = ft_strdup(wildcard_tokens->value);
	if (!merged)
		return (NULL);
	curr = wildcard_tokens->next;
	while (curr)
	{
		tmp = ft_strjoin(merged, " ", 1, 0);
		if (!tmp)
			return (NULL);
		merged = ft_strjoin(tmp, curr->value, 1, 0);
		if (!merged)
			return (NULL);
		curr = curr->next;
	}
	return (merged);
}

static t_token	*create_regular_token(t_token *tmp)
{
	t_token	*new;
	char	*str_dup;

	str_dup = ft_strdup(tmp->value);
	if (!str_dup)
		return (NULL);
	new = token_new(tmp->token_type, str_dup,
			tmp->single_quoted, tmp->double_quoted);
	if (!new)
		return (NULL);
	new->space_after = tmp->space_after;
	return (new);
}

static t_token	*create_merged_wildcard_token(t_token *tmp)
{
	t_token	*wildcard_tokens;
	t_token	*new;
	char	*merged_value;
	bool	had_space;

	wildcard_tokens = manage_wildcard(tmp);
	if (!wildcard_tokens)
		return (create_regular_token(tmp));
	merged_value = merge_wildcard_values(wildcard_tokens);
	if (!merged_value)
	{
		free_all_token(wildcard_tokens);
		return (NULL);
	}
	had_space = tmp->space_after;
	new = token_new(tmp->token_type, merged_value,
			tmp->single_quoted, tmp->double_quoted);
	if (!new)
		return (NULL);
	new->space_after = had_space;
	free_all_token(wildcard_tokens);
	return (new);
}

static bool	is_expandable_wildcard(t_token *token)
{
	if (token->token_type == WORD && ft_strchr(token->value, '*')
		&& !token->double_quoted
		&& token->expand_wildcard)
		return (true);
	return (false);
}

t_token	*expand_wildcard(t_token *tokens)
{
	t_token	*tmp;
	t_token	*expanded_tokens;
	t_token	*new;

	if (!tokens)
		return (NULL);
	tmp = tokens;
	expanded_tokens = NULL;
	while (tmp)
	{
		if (is_expandable_wildcard(tmp))
			new = create_merged_wildcard_token(tmp);
		else
			new = create_regular_token(tmp);
		if (!new)
			return (free_all_token(expanded_tokens), NULL);
		token_addback(&expanded_tokens, new);
		tmp = tmp->next;
	}
	return (expanded_tokens);
}


t_token	*manage_wildcard(t_token *to_expand)
{
	DIR				*dir;
	struct dirent	*entry;
	t_token			*expanded_tokens;
	t_token			*new;
	char			*path;
	char			*tmp;

	expanded_tokens = NULL;
	dir = opendir(".");
	if (!dir)
		return (NULL);
	while ((entry = readdir(dir)))
	{
		if (ft_match_pattern(entry->d_name, to_expand->value))
		{
			
			path = ft_strjoin("", entry->d_name, 0, 0);
			tmp = ft_strjoin(path, "", 1, 0);
			new = token_new(WORD, tmp, to_expand->single_quoted, to_expand->double_quoted);
			new->expand_wildcard = false;
			token_addback(&expanded_tokens, new);
		}
	}
	closedir(dir);
	return (expanded_tokens);
}