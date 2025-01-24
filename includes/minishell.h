/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 13:48:17 by macorso           #+#    #+#             */
/*   Updated: 2025/01/24 17:07:31 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

# define MINISHELL_H

# include "../libft/libft.h"

# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <unistd.h>

typedef enum e_token
{
	WORD,
	SEMICOLON,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
}	t_token;

typedef struct s_env
{
	char		*key; // name before the = sign
	char		*value; // name after the = sign
	bool		have_equal;
	struct s_env	*next;
}	t_env;

typedef struct s_all
{
	t_env	*env;
}	t_all;

void	print_header(void);

//env
char	*find_key(char *envp_value);
char	*find_value(char *envp_value);

#endif