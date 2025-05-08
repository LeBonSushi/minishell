/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jd-halle <jd-halle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:44:28 by jd-halle          #+#    #+#             */
/*   Updated: 2025/05/08 19:56:50 by jd-halle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void set_execution_mode(void)
{
    g_zebi.g_in_cmd_execution = 1;
    
    // Ignorer les signaux pendant l'exécution dans le processus parent
    // pour éviter que le shell parent ne soit affecté par Ctrl+C dans le minishell enfant
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
}

void set_interactive_mode(void)
{
    g_zebi.g_in_cmd_execution = 0;
    
    // Restaurer les gestionnaires de signaux normaux
    setup_signals();
}