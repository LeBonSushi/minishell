/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 18:32:46 by macorso           #+#    #+#             */
/*   Updated: 2025/01/27 15:26:54 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	echo_builtin(char **strs, bool n_option)
{
	int	i;

	i = 1;
	while (strs[i])
	{
		printf("%s", strs[i]);
		if (strs[i + 1])
			printf(" ");
		i++;
	}
	if (!n_option)
		printf("\n");
}