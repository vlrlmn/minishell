
NAME = minishell
CC = cc --debug -Wall -Wextra -Werror -g3
CFLAGS = -I/usr/local/Cellar/readline/8.1/include
LDFLAGS = -lreadline -L/usr/local/Cellar/readline/8.1/lib

SRC = 			main.c \
				errors.c \
				free_memory.c \

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
			./exec/redirection/heredoc_utils.c \
			./exec/redirection/redir_cmdlist.c \
			./exec/redirection/redirection.c \
			./exec/redirection/redir_utils.c \
			./exec/redirection/redir_cmdlist_utils.c \

SRC_UTILS = ./exec/utils/utils_env.c \
			./exec/utils/exec_utils.c \
			./exec/utils/utils_fill_cmd.c \
			./exec/utils/utils_list.c \
			./exec/utils/exec_path_utils.c \
			./exec/utils/expantion.c \
			./exec/utils/free_utils.c \


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
