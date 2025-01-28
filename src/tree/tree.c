/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dfeve <dfeve@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 22:25:10 by dfeve             #+#    #+#             */
/*   Updated: 2025/01/27 22:36:18 by dfeve            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_tree	*new_tree(t_tree *left_child, t_tree *right_child, char value, t_tree_flag flag)
{
	t_tree	*result;

	result = malloc(sizeof(t_tree));
	result->left_child = left_child;
	result->right_child = right_child;
	result->value = value;
	result->flag = flag;
}