/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macorso <macorso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 12:10:38 by lpolizzi          #+#    #+#             */
/*   Updated: 2025/03/04 23:05:29 by macorso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

int	ft_print_char(const int c, int fd)
{
	write(fd, (unsigned char *)&c, 1);
	return (1);
}

int	ft_formats(va_list args, const char format, int fd)
{
	int	len;

	len = 0;
	if (format == 'c')
		len += ft_print_char(va_arg(args, int), fd);
	else if (format == 's')
		len += ft_print_str(va_arg(args, char *), fd);
	else if (format == 'p')
		len += ft_print_ptr(va_arg(args, void *), fd);
	else if (format == 'd' || format == 'i')
		len += ft_print_nbr(va_arg(args, int), fd);
	else if (format == 'u')
		len += ft_print_unsigned(va_arg(args, unsigned int), fd);
	else if (format == 'x' || format == 'X')
		len += ft_print_hex(va_arg(args, unsigned int), format, fd);
	else if (format == '%')
		len += ft_print_percent(fd);
	return (len);
}

int	ft_fprintf(int fd, const char *str, ...)
{
	int		i;
	va_list	args;
	int		len;

	i = 0;
	len = 0;
	va_start(args, str);
	while (str[i])
	{
		if (str[i] == '%')
		{
			len += ft_formats(args, str[i + 1], fd);
			i++;
		}
		else
			len += write(fd, &str[i], 1);
		i++;
	}
	va_end(args);
	return (len);
}
/*
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main() {
    printf("Testing custom ft_printf vs standard printf\n\n");

    // Edge cases
    printf("Test 1: Characters\n");
    ft_printf("Character: %c\n", 'A');
    ft_printf("Character: %c\n\n", '\0'); // Null character
    printf("Character: %c\n", 'A');
    printf("Character: %c\n", '\0');

	char *str = 0;
    printf("\nTest 2: Strings\n");
    ft_printf("String: %s\n", "Hello, World!");
    ft_printf("String: %s\n", ""); // Empty string
    ft_printf("String: %s\n\n", str); // Null string
    printf("String: %s\n", "Hello, World!");
    printf("String: %s\n", ""); // Empty string
    printf("String: %s\n", str); // Null string

    printf("\nTest 3: Integers\n");
    ft_printf("Integer: %d\n", 42);
    ft_printf("Integer: %d\n", -42);
    ft_printf("Integer: %d\n\n", 0);
    printf("Integer: %d\n", 42);
    printf("Integer: %d\n", -42);
    printf("Integer: %d\n", 0);
    
    printf("\nTest 4: Unsigned Integers\n");
    ft_printf("Unsigned: %u\n", 42);
    ft_printf("Unsigned: %u\n", -42); // should handle as unsigned
    ft_printf("Unsigned: %u\n\n", 0);
    printf("Unsigned: %u\n", 42);
    printf("Unsigned: %u\n", -42); // should handle as unsigned
    printf("Unsigned: %u\n", 0);

    printf("\nTest 5: Pointers\n");
    int var = 42;
    ft_printf("Pointer: %p\n", (void*)&var); // Pointer to an integer
    ft_printf("Pointer: %p\n\n", NULL); // Null pointer
    printf("Pointer: %p\n", (void*)&var); // Pointer to an integer
    printf("Pointer: %p\n", NULL); // Null pointer

    printf("\nTest 6: Hexadecimal\n");
    ft_printf("Hexadecimal (lowercase): %x\n", 255);
    ft_printf("Hexadecimal (uppercase): %X\n\n", 255);
    printf("Hexadecimal (lowercase): %x\n", 255);
    printf("Hexadecimal (uppercase): %X\n", 255);
    
    printf("\nTest 7: Percent Sign\n");
    ft_printf("Percent sign: %%\n\n");
    printf("Percent sign: %%\n");

    printf("\nTest 8: Large Numbers\n");
    ft_printf("Large positive: %d\n", INT_MAX);
    ft_printf("Large negative: %d\n\n", INT_MIN);
    printf("Large positive: %d\n", INT_MAX);
    printf("Large negative: %d\n", INT_MIN);
    
    printf("\nTest 9: Mixed Formats\n");
    ft_printf("Mixed: %d, %u, %s, %p\n\n", -1, 1u, "Test", (void*)0x12345678);
    printf("Mixed: %d, %u, %s, %p\n", -1, 1u, "Test", (void*)0x12345678);

    return 0;
}*/
