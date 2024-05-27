# cc main.c -I/usr/local/opt/readline/include -L/usr/local/opt/readline/lib -lreadline -o minishell

# 1. -lreadline - compilation flag
# 2. -I/usr/local/opt/readline/include -L/usr/local/opt/readline/lib -lreadline libraries for functions like rl_replace_line and rl_redisplay 

NAME = minishell
CC = cc -Wall -Wextra -Werror
CFLAGS = -I/usr/local/opt/readline/include
LDFLAGS = -L/usr/local/opt/readline/lib -lreadline

SRC = 	main.c \
		errors.c \
		free_memory.c \
		valid_input.c \
		parsing/lexer_utils.c \
		parsing/parsing.c \
		parsing/null_terminator.c \
		parsing/parsing_utils.c \
		parsing/parse_redir.c \
		parsing/fill_tree.c \
		exec/exec_utils.c \
		exec/run_cmd.c \
		exec/exec_path_cmd.c \
		exec/exec_buildin.c \
		exec/cd.c \
		exec/echo.c \
		exec/pwd.c \
		exec/export.c \

OBJ = $(SRC:.c=.o)

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
