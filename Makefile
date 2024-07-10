# cc main.c -I/usr/local/opt/readline/include -L/usr/local/opt/readline/lib -lreadline -o minishell

# 1. -lreadline - compilation flag
# 2. -I/usr/local/opt/readline/include -L/usr/local/opt/readline/lib -lreadline libraries for functions like rl_replace_line and rl_redisplay 

NAME = minishell
CC = cc --debug -Wall -Wextra -Werror -g3 -fsanitize=address
CFLAGS = -I/usr/local/opt/readline/include
# LDFLAGS = -I/usr/local/include -L/usr/local/lib -lreadline  
LDFLAGS = -lreadline  
# -L/usr/local/opt/readline/lib -lreadline

SRC = 			main.c \
				errors.c \
				free_memory.c

SRC_PARSING = 	./parsing/parser/parsing.c \
				./parsing/parser/parse_exec.c \
				./parsing/parser/null_terminator.c \
				./parsing/parser/parsing_utils.c \
				./parsing/parser/parse_redir.c \
				./parsing/parser/fill_tree.c \
				./parsing/parser/tokens_utils.c \
				./parsing/lexer/lexer_expander.c \
				./parsing/lexer/lexer_quotes.c \
				./parsing/lexer/lexer_list.c \
				./parsing/lexer/lexer.c \
				./parsing/lexer/lexer_utils.c \
				./parsing/syntax_check/valid_syntax.c \
				./parsing/syntax_check/valid_input.c \
				./parsing/syntax_check/valid_syntax_utils.c \

SRC_EXECUTION = ./exec/execution.c \
				./exec/exec_buildin.c \
				./exec/signals.c \
				./exec/create_list.c \
				./exec/built_connections.c \
				
				
SRC_BUILTIN = ./exec/builtins/cd.c \
			./exec/builtins/echo.c \
			./exec/builtins/pwd.c \
			./exec/builtins/export.c \
			./exec/builtins/export_utils.c \
			./exec/builtins/env.c \
			./exec/builtins/unset.c \
			./exec/builtins/exit.c \


SRC_REDIR = ./exec/redirection/heredoc.c \
			./exec/redirection/redir_cmdlist.c \
			./exec/redirection/redirection.c \

SRC_UTILS = ./exec/utils/utils_env.c \
			./exec/utils/utils_fill_cmd.c \
			./exec/utils/utils_list.c \
			./exec/utils/exec_utils.c \
			./exec/utils/expantion.c \

SRCS = $(SRC) $(SRC_PARSING) $(SRC_EXECUTION) $(SRC_BUILTIN) $(SRC_REDIR) $(SRC_UTILS)

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
