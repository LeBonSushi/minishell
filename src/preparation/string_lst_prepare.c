/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_lst_prepare.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 02:45:16 by macorso           #+#    #+#             */
/*   Updated: 2025/02/15 02:47:19 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_string_list	*ft_strnew(char *content, t_redir_type redir_type)
{
	t_string_list	*new;

	new = (t_string_list *)malloc(sizeof(t_string_list));
	if (!new)
		return (NULL);
	new->str = content;
	new->redir_type = redir_type;
	new->next = NULL;
	return (new);
}

t_string_list	*ft_strlast(t_string_list *lst)
{
	t_string_list	*last;

	last = lst;
	if (!last)
		return (NULL);
	while (last->next)
		last = last->next;
	return (last);
}

void	ft_stradd_back(t_string_list **lst, t_string_list *new)
{
	t_string_list	*last;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	last = ft_strlast(*lst);
	last->next = new;
}