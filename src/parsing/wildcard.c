/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 22:33:53 by macorso           #+#    #+#             */
/*   Updated: 2025/02/18 02:04:45 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*merge_wildcard_values(t_token *wildcard_tokens)
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

t_token	*create_regular_token(t_token *tmp)
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

t_token	*create_merged_wildcard_token(t_token *tmp)
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

bool	is_expandable_wildcard(t_token *token)
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
	// print_tokens(tokens);
	while (tmp)
	{
		if (is_expandable_wildcard(tmp))
		{
			new = create_merged_wildcard_token(tmp);
			edit_token(&new, WORD, new->value);
		}
		else
			new = create_regular_token(tmp);
		if (!new)
			return (free_all_token(expanded_tokens), NULL);
		token_addback(&expanded_tokens, new);
		tmp = tmp->next;
	}
	return (expanded_tokens);
}


bool	is_hidden_valid(char *str, char *pattern)
{
	if (*str == '.' && *pattern != '.')
		return (false);
	return (true);
}

bool	ft_match_pattern(char *str, char *pattern)
{
	if (!*pattern)
		return (!*str);
	if (!is_hidden_valid(str, pattern))
		return (false);
	if (*pattern == '*' && *(pattern + 1) == '*')
		return (ft_match_pattern(str, pattern + 1));
	if (*pattern == '*')
		return (ft_match_pattern(str, pattern + 1)
			|| (*str && ft_match_pattern(str + 1, pattern)));
	if (*pattern == '?' && *str)
		return (ft_match_pattern(str + 1, pattern + 1));
	if (*pattern == *str)
		return (ft_match_pattern(str + 1, pattern + 1));
	return (false);
}

void	sort_matches(char **matches, int count)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strcmp(matches[j], matches[j + 1]) > 0)
			{
				temp = matches[j];
				matches[j] = matches[j + 1];
				matches[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

t_token	*create_tokens_from_matches(char **matches,
				t_token *to_expand)
{
	t_token	*expanded_tokens;
	t_token	*new;
	int		i;

	expanded_tokens = NULL;
	i = 0;
	while (matches[i])
	{
		new = token_new(WORD, matches[i], to_expand->single_quoted,
				to_expand->double_quoted);
		if (!new)
		{
			free_all_token(expanded_tokens);
			return (NULL);
		}
		new->expand_wildcard = false;
		token_addback(&expanded_tokens, new);
		i++;
	}
	return (expanded_tokens);
}

char	**store_matches(DIR *dir, t_token *to_expand, int *match_count)
{
	struct dirent	*entry;
	char			**matches;
	int				i;

	*match_count = 0;
	while ((entry = readdir(dir)))
		if (ft_match_pattern(entry->d_name, to_expand->value))
			(*match_count)++;
	matches = (char **)malloc(sizeof(char *) * (*match_count + 1));
	if (!matches)
		return (NULL);
	rewinddir(dir);
	i = 0;
	while ((entry = readdir(dir)) && i < *match_count)
	{
		if (ft_match_pattern(entry->d_name, to_expand->value))
		{
			matches[i] = ft_strdup(entry->d_name);
			if (!matches[i])
				return (NULL);
			i++;
		}
	}
	matches[i] = NULL;
	return (matches);
}

t_token	*manage_wildcard(t_token *to_expand)
{
	DIR		*dir;
	char	**matches;
	t_token	*expanded_tokens;
	int		match_count;

	if (!to_expand || !to_expand->value)
		return (NULL);
	dir = opendir(".");
	if (!dir)
		return (NULL);
	matches = store_matches(dir, to_expand, &match_count);
	if (!matches)
	{
		closedir(dir);
		return (NULL);
	}
	sort_matches(matches, match_count);
	expanded_tokens = create_tokens_from_matches(matches, to_expand);
	closedir(dir);
	free(matches);
	return (expanded_tokens);
}