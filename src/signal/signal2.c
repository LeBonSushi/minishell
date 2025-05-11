/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jd-halle <jd-halle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:44:28 by jd-halle          #+#    #+#             */
/*   Updated: 2025/04/29 21:44:48 by jd-halle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	set_execution_mode(void)
{
	g_in_cmd_execution = 1;
}
void	set_interactive_mode(void)
{
	g_in_cmd_execution = 0;
}