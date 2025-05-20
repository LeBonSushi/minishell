CC		:= cc
CFLAGS	:= -Werror -Wextra -Wall -Iincludes/ -I/opt/homebrew/opt/readline/include -g3 #-fsanitize=address,leak
LDFLAGS	:= -L/opt/homebrew/opt/readline/lib -lreadline

NAME	:= minishell

SRC		:=	src/minishell.c \
			src/parsing/tokenizer.c \
			src/parsing/tokenizer2.c \
			src/parsing/tokenizer3.c \
			src/parsing/tokenizer4.c \
			src/parsing/expander.c \
			src/parsing/expander2.c \
			src/parsing/expander3.c \
			src/parsing/expander4.c \
\
			src/utils/free.c \
			src/utils/search.c \
			src/utils/atoll.c \
			src/utils/free_all.c \
			src/utils/fd.c \
			src/utils/init.c \
			src/utils/env_utils.c \
			src/utils/print_utils.c \
			src/utils/token_utils.c \
\
			src/builtins/ft_env.c \
			src/builtins/ft_export.c \
			src/builtins/ft_export2.c \
			src/builtins/ft_unset.c \
			src/builtins/ft_echo.c \
			src/builtins/ft_cd.c \
			src/builtins/ft_pwd.c \
			src/builtins/ft_exit.c \
			src/builtins/execute_builtins.c \
\
			src/preparation/env_prepare.c \
			src/preparation/token_prepare.c \
			src/preparation/cmd_prepare.c \
			src/preparation/prepare_exec.c \
\
			src/exec/redirections.c \
			src/exec/redirections2.c \
			src/exec/redirections3.c \
			src/exec/redirections4.c \
			src/exec/exec.c \
			src/exec/exec2.c \
			src/exec/exec3.c \
			src/exec/exec4.c \
\
			src/signal/signal.c \
			src/signal/signal_setm.c \
\

OBJ		:= $(SRC:.c=.o)

LIB		:=	libft/libft.a \

GREEN	:= "\033[0m\033[1;32m"
CYAN	:= "\033[0m\033[1;36m"
YELLOW	:= "\033[0m\033[1;33m\033[3;33m"
RESET	:= "\033[0m"

.c.o:
	@printf $(GREEN)"\r\033[KCompiling objects... "$(YELLOW)"<$<> ⏳ "$(RESET)
	@$(CC) $(CFLAGS) -c $< -o $@

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

all: $(NAME)

run: all
	@./$(NAME)

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

clean:
	@printf $(CYAN)"\r\033[KErasing objects... "$(RESET)"⏳ "
	@rm -rdf $(OBJ)
	@make -C libft clean --silent
	@printf $(GREEN)"\r\033[Kcleaned 🗑\n"$(RESET)

fclean:
	@printf $(CYAN)"\r\033[KErasing objects... "$(RESET)"⏳ "
	@rm -rdf $(OBJ)
	@make -C libft fclean --silent
	@printf $(GREEN)"\r\033[KObjects cleaned 🗑\n"$(RESET)
	@printf $(CYAN)"\r\033[KErasing binary file... "$(RESET)"⏳ "
	@rm -f $(NAME)
	@printf $(GREEN)"\r\033[KBinary file cleaned 🗑\n"$(RESET)
	@printf $(GREEN)"\r\033[KForce cleaned 🗑\n"$(RESET)

re: fclean all

.PHONY: all clean fclean re  load run
