NAME = minishell
CC = cc
FLAGS = -Wall -Wextra -Werror -g -fsanitize=address
RM = rm -rf

#BONUS_NAME = checker
LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

# Include path for header files
INCLUDES = -I./includes

GREEN = \e[38;5;114m
ROSE = \e[38;5;218m
BLUE = \e[38;5;153m
NC = \e[0m

SRCS =  src/minishell.c \

OBJS = $(SRCS:.c=.o)

#BONUS_OBJS = $(BONUS_SRCS:.c=.o)

$(NAME): $(LIBFT) $(OBJS)
	@echo "$(ROSE)\e[1m┌─────$(NAME)──────────────────────────────────┐\e"
	@echo "││$(BLUE)         Compiling $(NAME) 🌀        $(ROSE) │"
	@echo "\e[1m└────────────────────────────────────────────────┘\e"
	@$(CC) $(FLAGS) $(INCLUDES) $(OBJS) $(LIBFT) -o $(NAME)
	@echo "$(RED)\e[1m┌─────$(NAME)──────────────────────────────────┐\e"
	@echo "││$(GREEN)        \e[1mCompilation finished 🌱\e      $(ROSE) │"
	@echo "\e[1m└────────────────────────────────────────────────┘\e"

#$(BONUS_NAME): $(LIBFT) $(BONUS_OBJS)
#    @echo "$(ROSE)\e[1m┌─────$(BONUS_NAME)──────────────────────────────┐\e"
#    @echo "││$(BLUE)         Compiling $(BONUS_NAME) 🎯  $(ROSE) │"
#    @echo "\e[1m└────────────────────────────────────────────────┘\e"
#    @$(CC) $(FLAGS) -o $(BONUS_NAME) $(BONUS_OBJS) $(LIBFT)
#    @echo "$(GREEN)\e[1m┌─────$(BONUS_NAME)──────────────────────────────┐\e"
#    @echo "││$(GREEN)        \e[1mCompilation finished 🌟\e  $(ROSE) │"
#    @echo "\e[1m└────────────────────────────────────────────────┘\e"

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR) --no-print-directory
	
.c.o:
	@$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

all: $(NAME)

#bonus: $(BONUS_NAME)

clean:
	@$(RM) $(OBJS) #$(BONUS_OBJS)
	@$(MAKE) -C $(LIBFT_DIR) clean --no-print-directory

fclean: clean
	@$(RM) $(NAME) #$(BONUS_OBJS)
	@$(MAKE) -C $(LIBFT_DIR) fclean --no-print-directory

re: fclean all
.PHONY: all bonus clean fclean re