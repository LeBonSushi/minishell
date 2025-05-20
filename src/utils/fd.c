/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 14:54:44 by lebonsushi        #+#    #+#             */
/*   Updated: 2025/05/20 19:14:00 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_fds(t_cmd *cmd)
{
	if (cmd->infile_fd > 0)
	{
		dup2(cmd->infile_fd, STDIN_FILENO);
		close(cmd->infile_fd);
	}
	if (cmd->outfile_fd > 0)
	{
		dup2(cmd->outfile_fd, STDOUT_FILENO);
		close(cmd->outfile_fd);
	}
}
