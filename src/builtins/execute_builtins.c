/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtins.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jd-halle <jd-halle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 01:58:22 by jd-halle          #+#    #+#             */
/*   Updated: 2025/02/26 03:40:34 by jd-halle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void execute_builtins(t_token *tokens, t_env **env, t_all *all)
{
    if (!tokens)
        return;
        
    // Make a copy of the token value to avoid use-after-free if token is modified by builtin
    if (!ft_strcmp(tokens->value, "echo"))
        ft_echo(tokens);
    else if (!ft_strcmp(tokens->value, "cd"))
        ft_cd(tokens, *env);
    else if (!ft_strcmp(tokens->value, "pwd"))
        ft_pwd();
    else if (!ft_strcmp(tokens->value, "c"))
        clear_terminal();
    else if (!ft_strcmp(tokens->value, "export"))
        ft_export(tokens, env);
    else if (!ft_strcmp(tokens->value, "env"))
        ft_env(all->env_no_sorted);
    else if (!ft_strcmp(tokens->value, "exit"))
        ft_exit(tokens, all);
    /*else if (!ft_strcmp(tokens->value, "unset"))
        ft_unset(args);*/
        
}