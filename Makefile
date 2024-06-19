# cc main.c -I/usr/local/opt/readline/include -L/usr/local/opt/readline/lib -lreadline -o minishell

# 1. -lreadline - compilation flag
# 2. -I/usr/local/opt/readline/include -L/usr/local/opt/readline/lib -lreadline libraries for functions like rl_replace_line and rl_redisplay 

NAME = minishell
CC = cc --debug -Wall -Wextra -Werror -g3 -fsanitize=address
CFLAGS = -I/usr/local/opt/readline/include
LDFLAGS = -I/usr/local/include -L/usr/local/lib -lreadline  
# -L/usr/local/opt/readline/lib -lreadline

SRC_PARSING = ./parsing/lexer_utils.c \
				./parsing/parsing.c \
				./parsing/null_terminator.c \
				./parsing/parsing_utils.c \
				./parsing/parse_redir.c \
				./parsing/fill_tree.c \
				./parsing/lexer.c \
				./parsing/lexer_list.c \
				./parsing/lexer_expander.c \
				./parsing/lexer_quotes.c \

SRC_BUILTIN = ./exec/builtins/cd.c \
			./exec/builtins/echo.c \
			./exec/builtins/pwd.c \
			./exec/builtins/export.c \
			./exec/builtins/env.c \
			./exec/builtins/unset.c \

SRC_EXECUTION = ./exec/exec_utils.c \
				./exec/run_cmd.c \
				./exec/redirections.c \
				./exec/exec_path_cmd.c \
				./exec/exec_buildin.c \
				./exec/utils_env.c \
				./exec/utils_list.c \
				./exec/utils_fill_cmd.c \
				./exec/create_list.c \

SRC = 			main.c \
				errors.c \
				free_memory.c \
				valid_input.c \

SRCS = $(SRC) $(SRC_PARSING) $(SRC_EXECUTION) $(SRC_BUILTIN)

OBJ = $(SRCS:.c=.o)

HDRS = minishell.h

RM = rm -f

LIBFT = Libft/libft.a

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME):	$(OBJ)
	$(MAKE) -C Libft
	$(CC) -o $(NAME) $(OBJ) $(LIBFT) $(LDFLAGS)

all:		$(NAME)

clean:
	$(RM) $(OBJ)
	$(MAKE) -C Libft clean

fclean:	clean
	$(RM) $(NAME)
	$(MAKE) -C Libft fclean

re:		fclean all

.PHONY:	all clean fclean re
