/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 11:11:39 by macorso           #+#    #+#             */
/*   Updated: 2025/05/21 11:11:47 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_redir(t_token *token)
{
	return (token->token_type == REDIR_IN
		|| token->token_type == REDIR_OUT
		|| token->token_type == REDIR_APPEND
		|| token->token_type == REDIR_HEREDOC);
}