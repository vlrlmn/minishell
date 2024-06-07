/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 12:43:09 by vlomakin          #+#    #+#             */
/*   Updated: 2024/06/07 13:18:22 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL
# define MINISHELL

// # include "Libft/libft.h"
// # include "printf/libftprintf.h"
# include "Libft/libft.h"
# include <limits.h>
# include <math.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sysexits.h> //for EX_UNAVAILABLE
# include <termios.h>
# include <unistd.h>
# include <fcntl.h>

# define MALLOC_ERROR 69
# define SYNTAX_ERR 2
# define MAXARGS 1024

typedef struct s_args
{
	char	*input;
	char	**envp;
}			t_args;

typedef struct s_cmd
{
	int		type;
}			t_cmd;

typedef struct s_execmd
{
	int		type;
	char	*argv[MAXARGS];
	char	*eargv[MAXARGS];
	// char	**envp;
}			t_execcmd;


typedef struct s_pipe
{
	int		type;
	char	*argv[MAXARGS];
	char	*eargv[MAXARGS];
	t_cmd	*left;
	t_cmd	*right;
	// char	**envp;
}			t_pipe;

typedef struct s_redir
{
	int		type;
	char	*argv[MAXARGS];
	char	*eargv[MAXARGS];
	t_cmd	*cmd;
	char	*file;
	char	*efile;
	int		mode;
	int		fd;
}			t_redir;

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
	PIPE = 0,
	EXEC = 1,
	REDIR = 2
}			t_type;

/*Errors and free*/
int cd_cmd(t_execcmd *ecmd);
int echo_cmd(t_execcmd *ecmd);
int pwd_cmd(t_execcmd *ecmd);
int export_cmd(t_execcmd *ecmd, t_args *params);
void		lexical_analysis(t_cmd *cmd, t_args *args);
t_cmd		*nulterminate(t_cmd *cmd);
int			valid_input(char *work_line);
int			fork1(void);
void			free_split(char **arr);
void			run_cmd(t_cmd *cmd, t_args *params);
int				run_buildin(t_execcmd	*ecmd, t_args *params);
char			*find_command_path(char *cmd, char *path);
int			gettoken(char **ps, char *es, char **q, char **eq);
void		exit_with_err(char *msg);
int			peek(char **ps, char *es, char *toks);
t_cmd		*parseredir(t_cmd *cmd, char **ps, char *es);
void		free_envp(t_args *args);
t_cmd		*pipecmd(t_cmd *left, t_cmd *right);
t_cmd		*parsepipe(char **ps, char *es);
void		free_envp(t_args *args);
t_cmd		*parse(t_args *args);
void		panic_and_free_env(t_args *args, int index);
void		exit_with_syntax_err(t_args *args, int err_code);
void		exit_with_malloc_error(int err_code);
int			is_delimiter(char c);
int			ft_isalnum(int c);
int			is_symbol(char c);

void PrintTree(t_cmd	*cmd);
#endif