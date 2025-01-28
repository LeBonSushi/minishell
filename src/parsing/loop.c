/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dfeve <dfeve@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 21:41:59 by dfeve             #+#    #+#             */
/*   Updated: 2025/01/28 03:16:20 by dfeve            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	minishell_loop(t_all *all)
{
	env_builtin(all);
	char *line;
	t_command	*command;
	while (1)
	{
		line = readline("hello:");
		command = parse_str_to_command(line);
		print_token(command->token);
	}
}