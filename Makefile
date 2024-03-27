cc main.c -I/usr/local/opt/readline/include -L/usr/local/opt/readline/lib -lreadline -o minishell

1. -lreadline - compilation flag
2. -I/usr/local/opt/readline/include -L/usr/local/opt/readline/lib -lreadline libraries for functions like rl_replace_line and rl_redisplay 

NAME = minishell

CC = cc -Wall -Wextra -Werror

SRC = 	main.c \
		lexer.c \
		error.c \

OBJ = $(SRC:.c=.o)

HDRS = ../minishell.h

RM = rm -f

LIBFT = Libft/libft.a

PRINTF = printf/libftprintf.a

.c.o:
	${CC} -I ${HDRS} -c $< -o ${<:.c=.o}

$(NAME):		$(OBJ)
				$(MAKE) -C printf
				$(MAKE) -C Libft
				${CC} -o $(NAME) $(OBJ) $(LIBFT) $(PRINTF)

all:			$(NAME)

clean:
			${RM} $(OBJ)
			$(MAKE) -C printf clean
			$(MAKE) -C Libft clean


fclean:		clean
		${RM} $(NAME)
		$(MAKE) -C printf fclean
		$(MAKE) -C Libft fclean

re:			fclean all

.PHONY:		all clean fclean re