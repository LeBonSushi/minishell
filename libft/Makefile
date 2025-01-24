# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: macorso <macorso@student.42nice.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/10 17:19:22 by macorso          #+#    #+#              #
#    Updated: 2024/11/11 11:26:02 by macorso         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

COMP = cc
CFLAGS = -Wall -Wextra -Werror -I ./
NAME = libft.a
ADDIDIR = additional
LIBCDIR = libc
BONUSDIR = bonus
PRINTFDIR = printf
GNLDIR = gnl

LIBC =		libc/ft_atoi.c					libc/ft_bzero.c					libc/ft_isalnum.c			libc/ft_isalpha.c		\
				libc/ft_isascii.c				libc/ft_isdigit.c					libc/ft_isprint.c			libc/ft_memchr.c		\
				libc/ft_memcmp.c				libc/ft_memcpy.c					libc/ft_memmove.c			libc/ft_memset.c		\
				libc/ft_strchr.c				libc/ft_strdup.c					libc/ft_strlcat.c			libc/ft_strlcpy.c		\
				libc/ft_strlen.c				libc/ft_strncmp.c					libc/ft_strnstr.c			libc/ft_strrchr.c		\
				libc/ft_tolower.c				libc/ft_toupper.c					libc/ft_calloc.c			libc/ft_strcmp.c		\
				libc/ft_strstr.c
	
ADDI =  	additional/ft_strjoin.c				additional/ft_substr.c					additional/ft_strtrim.c			additional/ft_split.c		\
				additional/ft_itoa.c					additional/ft_strmapi.c					additional/ft_striteri.c			additional/ft_putchar_fd.c		\
				additional/ft_putstr_fd.c			additional/ft_putendl_fd.c				additional/ft_putnbr_fd.c

BONUS = 	bonus/ft_lstnew_bonus.c			bonus/ft_lstadd_front_bonus.c		bonus/ft_lstsize_bonus.c	bonus/ft_lstlast_bonus.c	\
				bonus/ft_lstadd_back_bonus.c	bonus/ft_lstdelone_bonus.c			bonus/ft_lstclear_bonus.c	bonus/ft_lstiter_bonus.c	\
				bonus/ft_lstmap_bonus.c

PRINTF = 	printf/srcs/ft_printf.c				printf/srcs/ft_printf_utils.c				printf/srcs/ft_print_hex.c		printf/srcs/ft_print_ptr.c		\
				printf/srcs/ft_print_unsigned.c
		
GNL = 		gnl/srcs/get_next_line.c

SRCS = $(LIBC) $(ADDI) $(BONUS) $(PRINTF) $(GNL)

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
						ar rcs $(NAME) $(OBJS)

%.c%.o:
						$(COMP) $(CFLAGS) -c $< -o $@

clean:
						rm -rf $(OBJS)

fclean: clean
						rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean
