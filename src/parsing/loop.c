/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 21:41:59 by dfeve             #+#    #+#             */
/*   Updated: 2025/01/28 04:40:01 by macorso          ###   ########.fr       */
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
		line = readline("ministress> ");
		command = parse_str_to_command(line);
		print_tab(command->args);
		print_token(command->token);
	}
}