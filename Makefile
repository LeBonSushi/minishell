CC		:= cc
CFLAGS	:= -Werror -Wextra -Wall -I/opt/homebrew/opt/readline/include -g -fsanitize=address
LDFLAGS	:= -L/opt/homebrew/opt/readline/lib -lreadline

# Name of the final executable
NAME	:= minishell

# All needed files
SRC		:=	src/minishell.c \
\
			src/parsing/tokenizer.c \
\
			src/utils/header.c \
			src/utils/free.c \
			src/utils/print_utils.c \
\
			src/builtins/env.c \
			src/builtins/export.c \
			src/builtins/unset.c \
			src/builtins/echo.c \
			src/builtins/pwd.c \
			src/builtins/exit.c \
\
			src/preparation/env_prepare.c \
			src/preparation/token_prepare.c \

OBJ		:= $(SRC:.c=.o)

# All needed library
LIB		:=	libft/libft.a \

# Colors for differents prints
GREEN	:= "\033[0m\033[1;32m"
CYAN	:= "\033[0m\033[1;36m"
YELLOW	:= "\033[0m\033[1;33m\033[3;33m"
RESET	:= "\033[0m"

# Compile all .c files
.c.o:
	@printf $(GREEN)"\r\033[KCompiling objects... "$(YELLOW)"<$<> ⏳ "$(RESET)
	@$(CC) $(CFLAGS) -c $< -o $@

# Compile all .o files
$(NAME): $(OBJ)
	@printf $(GREEN)"\r\033[KObjects compiled succesfully ✅\n"$(RESET)
	@make -C libft --silent
	@printf $(CYAN)"\r\033[KCompiling '$(NAME)'... ⏳"$(RESET)
	@$(CC) $(CFLAGS) $(OBJ) $(LIB) $(LDFLAGS) -I includes/ -o $(NAME)
	@printf $(GREEN)"\r\033[KSuccess compiling '$(NAME)' ✅\n"$(RESET)
	@printf "\n"
	@printf "███╗   ███╗██╗███╗   ██╗██╗███████╗███████╗████████╗ ███████╗ ███████╗ ███████╗\n"
	@printf "████╗ ████║██║████╗  ██║██║██╔════╝  ███╔═╝██    ██║ ██╔════╝ ██╔════╝ ██╔════╝\n"
	@printf "██╔████╔██║██║██╔██╗ ██║██║███████╗  ███║  ████████║ █████╗   ███████╗ ███████╗\n"
	@printf "██║╚██╔╝██║██║██║╚██╗██║██║╚════██║  ███║  ██  ███═╝ ██╔══╝   ╚════██║ ╚════██║\n"
	@printf "██║ ╚═╝ ██║██║██║ ╚████║██║███████║  ███║  ██    ██  ███████╗ ███████║ ███████║\n"
	@printf "╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚══════╝  ╚══╝  ╚══════╝  ╚══════╝ ╚══════╝ ╚══════╝\n"


# Default command to launch
all: $(NAME) #load

# Compile and run minishell
run: all
	@./$(NAME)

# Just a cute loading bar
load:
	@printf '\r █▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒ - 0%%\r'
	@sleep .1
	@printf '\r ██▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒ - 5%%\r'
	@sleep .1
	@printf '\r ███▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒ - 10%%\r'
	@sleep .1
	@printf '\r ████▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒ - 15%%\r'
	@sleep .1
	@printf '\r █████▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒ - 20%%\r'
	@sleep .1
	@printf '\r ██████▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒ - 25%%\r'
	@sleep .1
	@printf '\r ███████▒▒▒▒▒▒▒▒▒▒▒▒▒▒ - 30%%\r'
	@sleep .1
	@printf '\r ████████▒▒▒▒▒▒▒▒▒▒▒▒▒ - 35%%\r'
	@sleep .1
	@printf '\r █████████▒▒▒▒▒▒▒▒▒▒▒▒ - 40%%\r'
	@sleep .1
	@printf '\r ██████████▒▒▒▒▒▒▒▒▒▒▒ - 45%%\r'
	@sleep .1
	@printf '\r ███████████▒▒▒▒▒▒▒▒▒▒ - 50%%\r'
	@sleep .1
	@printf '\r ████████████▒▒▒▒▒▒▒▒▒ - 55%%\r'
	@sleep .1
	@printf '\r █████████████▒▒▒▒▒▒▒▒ - 60%%\r'
	@sleep .1
	@printf '\r ██████████████▒▒▒▒▒▒▒ - 65%%\r'
	@sleep .1
	@printf '\r ███████████████▒▒▒▒▒▒ - 70%%\r'
	@sleep .1
	@printf '\r ████████████████▒▒▒▒▒ - 75%%\r'
	@sleep .1
	@printf '\r █████████████████▒▒▒▒ - 80%%\r'
	@sleep .1
	@printf '\r ██████████████████▒▒▒ - 85%%\r'
	@sleep .1
	@printf '\r ███████████████████▒▒ - 90%%\r'
	@sleep .1
	@printf '\r ████████████████████▒ - 95%%\r'
	@sleep .1
	@printf '\r █████████████████████ - 100%%\r'
	@sleep .8

# Clean all .o and .a files
clean:
	@printf $(CYAN)"\r\033[KErasing objects... "$(RESET)"⏳ "
	@rm -rdf $(OBJ)
	@make -C libft clean --silent
	@printf $(GREEN)"\r\033[Kcleaned 🗑\n"$(RESET)

# Same as 'clean' but clean minishell too
fclean:
	@printf $(CYAN)"\r\033[KErasing objects... "$(RESET)"⏳ "
	@rm -rdf $(OBJ)
	@make -C libft fclean --silent
	@printf $(GREEN)"\r\033[KObjects cleaned 🗑\n"$(RESET)
	@printf $(CYAN)"\r\033[KErasing binary file... "$(RESET)"⏳ "
	@printf $(GREEN)"\r\033[KBinary file cleaned 🗑\n"$(RESET)
	@printf $(GREEN)"\r\033[KForce cleaned 🗑\n"$(RESET)

# Clean all and recompile minishell
re: fclean all

.PHONY: all clean fclean re polnareff load run