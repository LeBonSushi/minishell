/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dfeve <dfeve@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 21:41:59 by dfeve             #+#    #+#             */
/*   Updated: 2025/01/27 23:30:54 by dfeve            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	minishell_loop(t_all *all)
{
	env_builtin(all);
	char *line;
	while (1)
	{
		line = readline("hello:");
		print_token(parse_str_to_command(line)->token);
	}
}