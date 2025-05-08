/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jd-halle <jd-halle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:44:28 by jd-halle          #+#    #+#             */
/*   Updated: 2025/05/08 01:29:27 by jd-halle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	set_execution_mode(void)
{
	g_zebi.g_in_cmd_execution = 1;
}

void	set_interactive_mode(void)
{
	g_zebi.g_in_cmd_execution = 0;
}