/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 23:22:28 by lpolizzi          #+#    #+#             */
/*   Updated: 2025/03/04 23:05:35 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <stdint.h>
# include "libft.h"

// Parameters functions for %[cspdiuxX%]
int	ft_print_unsigned(unsigned int n, int fd);
int	ft_print_nbr(int n, int fd);
int	ft_print_percent(int fd);
int	ft_print_str(char *str, int fd);
int	ft_print_ptr(void *ptr, int fd);
int	ft_print_hex(unsigned int n, const char format, int fd);

// Main ft_printf function
int	ft_fprintf(int fd, const char *str, ...);

#endif
