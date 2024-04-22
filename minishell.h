#ifndef MINISHELL
#define MINISHELL

// # include "Libft/libft.h"
// # include "printf/libftprintf.h"
# include <limits.h>
# include <math.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <termios.h>
#include <signal.h>
#include "Libft/libft.h"
#include <sysexits.h> //for EX_UNAVAILABLE

# define WORD 1
# define PIPE 2

# define SYNTAX_ERR 2

typedef struct s_token
{
    int             len;
    char            *begin;
    int             type;
}					t_token;

typedef struct s_tree
{
}					t_tree;

typedef struct s_args
{
    char            *input;
    char            **envp;
    t_tree          *tree;
    t_token         *tok;
}					t_args;

typedef enum quotes_handler
{
                    QUOTES_ERR,
                    MULTIPLE_QUOTES,
                    NO_QUOTES,
                    SINGLE_Q,
                    DOUBLE_Q,
}			t_quotes;


/*Lexer*/
void lexer(char *input);

/*Errors and free*/
void exit_with_syntax_err(int err_code);
void exit_with_malloc_error(int err_code);
void free_environment(t_args	*shell_context);
void    free_line_tokens(char **line_tokens);
#endif