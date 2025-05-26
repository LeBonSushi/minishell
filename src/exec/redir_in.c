/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:04:00 by macorso           #+#    #+#             */
/*   Updated: 2025/05/26 22:21:40 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redir_in(t_cmd *curr_cmd, t_token *curr_token)
{
	t_token	*file;
	char	*error;

	if (curr_cmd->infile_fd > 0)
		close(curr_cmd->infile_fd);
	file = curr_token->next;
	error = ft_strjoin("ministress: ", file->value, 0, 0);
	if (access(file->value, F_OK | R_OK))
	{
		curr_cmd->skip_cmd = true;
		return (perror(error), free(error), 1);
	}
	free(error);
	curr_cmd->infile_fd = open(file->value, O_RDONLY);
	return (0);
}
