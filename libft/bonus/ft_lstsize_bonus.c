/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpolizzi <lpolizzi@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 19:03:01 by lpolizzi          #+#    #+#             */
/*   Updated: 2024/10/13 20:22:05 by lpolizzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int		count;
	t_list	*curr;

	count = 0;
	if (!lst)
		return (0);
	curr = lst;
	while (curr)
	{
		count++;
		curr = curr->next;
	}
	return (count);
}
