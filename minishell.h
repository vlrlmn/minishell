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

# define SYNTAX_ERR 2
# define MAXARGS 10


typedef struct s_token
{
    int                 len;
    char                *value;
    int                 type;
    int                 pos;
    struct s_token      *next;
}					t_token;

typedef struct s_execmd
{
    int             type;
    char            *argv[MAXARGS];
    char            *eargv[MAXARGS];
}					t_tree;

typedef struct s_args
{
    char            *input;
    char            **envp;
    t_tree          *tree;
    t_token         *tok;
}					t_args;
typedef struct s_cmd
{
    int             type;
}					t_cmd;


typedef struct s_pipe
{
    int             type;
    t_cmd           *left;
    t_cmd           *right;
}					t_pipe;

typedef struct s_redir
{
    int             type;
    t_cmd           *cmd;
    char            *file;
    char            *efile;
    int             mode;
    int             fd;
}					t_redir;


typedef enum quotes_handler
{
                    QUOTES_ERR,
                    MULTIPLE_QUOTES,
                    NO_QUOTES,
                    SINGLE_Q,
                    DOUBLE_Q,
}			t_quotes;

typedef enum token_type
{
                    PIPE,
                    EXEC,
                    WORD,
                    WORD_IN_QUOTES,
                    REDIR_IN,
                    REDIR_OUT,
                    REDIR_APP,
                    HEREDOC,
                    EXPANSION,
}			t_type;
/*Lexer*/
void lexer(char *input);

/*Errors and free*/
void                    exit_with_syntax_err(int err_code);
void                    exit_with_malloc_error(int err_code);
void                    free_environment(t_args	*shell_context);
void                    free_line_tokens(char **line_tokens);
int	is_delimiter(char c);
void                    lexer(char *input);

/*Lexer tokenizers*/
int	word_token(char *work_line, int pos, t_token *tok);
int	expansion_token(char *work_line, int pos, t_token *tok);
void	two_symbols_token(char *symbols, int len, int pos, t_token *tok);
void	symbol_token(char symbol, int pos, t_token *tok);
int	in_quotes_token(char *line, int pos, char sym, t_token *tok);
#endif