# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: macorso <macorso@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/10 17:19:22 by lpolizzi          #+#    #+#              #
#    Updated: 2025/03/04 22:35:07 by macorso          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

COMP = cc
CFLAGS = -Wall -Wextra -Werror -fPIC -fPIE -I ./
NAME = libft.a
ADDIDIR = additional
LIBCDIR = libc
BONUSDIR = bonus
PRINTFDIR = printf
GNLDIR = gnl

LIBCFILE =		ft_atoi					ft_bzero					ft_isalnum			ft_isalpha		\
				ft_isascii				ft_isdigit					ft_isprint			ft_memchr		\
				ft_memcmp				ft_memcpy					ft_memmove			ft_memset		\
				ft_strchr				ft_strdup					ft_strlcat			ft_strlcpy		\
				ft_strlen				ft_strncmp					ft_strnstr			ft_strrchr		\
				ft_tolower				ft_toupper					ft_calloc			ft_strcmp		\
				ft_strnstr				ft_isspace					ft_strcpy

ADDIFILE =  	ft_strjoin				ft_substr					ft_strtrim			ft_split		\
				ft_itoa					ft_strmapi					ft_striteri			ft_putchar_fd		\
				ft_putstr_fd			ft_putendl_fd				ft_putnbr_fd

# BONUSFILE = 	ft_lstnew_bonus			ft_lstadd_front_bonus		ft_lstsize_bonus	ft_lstlast_bonus	\
# 				ft_lstadd_back_bonus	ft_lstdelone_bonus			ft_lstclear_bonus	ft_lstiter_bonus	\
# 				ft_lstmap_bonus

PRINTFFILE = 	ft_printf				ft_printf_utils				ft_print_hex		ft_print_ptr		\
				ft_print_unsigned

GNLFILE = 		get_next_line			get_next_line_utils

LIBC = $(addprefix $(LIBCDIR)/, $(addsuffix .c, $(LIBCFILE)))
ADDI = $(addprefix $(ADDIDIR)/, $(addsuffix .c, $(ADDIFILE)))
BONUS = $(addprefix $(BONUSDIR)/, $(addsuffix .c, $(BONUSFILE)))
PRINTF = $(addprefix $(PRINTFDIR)/srcs/, $(addsuffix .c, $(PRINTFFILE)))
GNL = $(addprefix $(GNLDIR)/srcs/, $(addsuffix .c, $(GNLFILE)))

SRCS = $(LIBC) $(ADDI) $(BONUS) $(PRINTF) $(GNL)

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
						ar rcs $(NAME) $(OBJS)

.c.o:
						$(COMP) $(CFLAGS) -c $< -o $@

clean:
						rm -rf $(OBJS)

fclean: clean
						rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean
