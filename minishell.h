/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 12:43:09 by vlomakin          #+#    #+#             */
/*   Updated: 2024/07/02 22:20:27 by lomakinaval      ###   ########.fr       */
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
# include <sys/wait.h>
# include <string.h>
# include <sys/stat.h>
#include <errno.h>

# define MALLOC_ERROR 69
# define SYNTAX_ERR 2
# define MAXARGS 1024
# define Y "\033[1;33m"   /* Bold Yellow for promt */
# define RST "\033[0m"      /* Reset to default color */

extern int g_exit_status; // CHECK IF IT IS CORRECT OR NOT

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
}			t_execcmd;

typedef struct s_pipe
{
	int		type;
	char	*argv[MAXARGS];
	char	*eargv[MAXARGS];
	t_cmd	*left;
	t_cmd	*right;
}			t_pipe;

typedef struct s_redir
{
	int		type;
	int		subtype;
	char	*argv[MAXARGS];
	char	*eargv[MAXARGS];
	t_cmd	*cmd;
	char	*file;
	char	*efile;
	int		mode;
	int		fd;
}			t_redir;

typedef struct s_lexem_node
{
	void			*data;
	struct s_lexem_node	*next;
}			t_lexem_node;

typedef struct s_lexems
{
	t_lexem_node		*head;
	t_lexem_node		*tail;
}			t_lexems;


typedef enum quotes_handler
{
	QUOTES_ERR,
	MULTIPLE_QUOTES,
	NO_QUOTES,
	SINGLE_Q,
	DOUBLE_Q,
}			t_quotes;

typedef enum redir_type
{
	HEREDOC = 3,
	APPEND = 4,
	REDIRIN = 5,
	REDIROUT = 6,
	NONE = -1
}	r_type;


typedef enum token_type
{
	PIPE = 0,
	EXEC = 1,
	REDIR = 2,
}			t_type;

/* if i have only one cmd, i don't need to create connections, only feel fd_read and fd_write. */
typedef struct s_cmd_info //free
{
	t_args	*args; //includes input and params
	int		head;
	int		index;
	int		type;
	int		redir_type; //heredoc or append
	t_cmd	*subcmd; //cmd to be executed in redirs
	// t_cmd	*cmd; insted of T_cmd it should point to the ... ????
	char	*argv[MAXARGS]; //to pass this fiels into execve()
	char	*eargv[MAXARGS];
	int		status; //?
	int		fd_read;
	int		fd_write;
	char	*file_read; //free if redir_type == HEREDOC
	char	*file_write;
	int		mode_read;
	int		mode_write;
	int		*connection; //free
	char	**hfile_array; //free
	struct s_cmd_info	*next;
}	t_cmd_info;

/*Errors and free*/
void		lexical_analysis(t_cmd *cmd, t_args *args);
void		old_check_arguments(t_execcmd *ecmd);

/*LEXER*/
void		parse_double_quote(int *i, char *line, t_lexems *list, t_args *args);
void		parse_quote(char *line, int *i, t_lexems *list);
void		parse_expander_sign(int *i, char *line, t_lexems *list, t_args *args);
void		parse_expander(int *i, t_lexems *list, char *line, t_args *args);

char		*get_env(char *value, char **envp);
t_cmd		*nulterminate(t_cmd *cmd);
int			valid_input(char *work_line);
int			fork1(void);
void			free_split(char **arr);
void			run_cmd(t_cmd *cmd, t_args *params);
char			*find_command_path(char *cmd, char *path);
int			gettoken(char **ps, char *es, char **q, char **eq);
void		exit_with_err(char *msg);
int			peek(char **ps, char *es, char *toks);
t_cmd		*parseredir(t_cmd *cmd, char **ps, char *es);
t_cmd		*pipecmd(t_cmd *left, t_cmd *right);
t_cmd		*parsepipe(char **ps, char *es);
t_cmd		*parse(t_args *args);
void		panic_and_free_env(t_args *args, int index);
void		exit_with_syntax_err(t_args *args, int err_code);
void		exit_with_malloc_error(int err_code);
int			is_delimiter(char c);
int			ft_isalnum(int c);
int			has_parse_symbol(char *s);
char		*clean_cmd (char *line, t_args *args);
void		add_char_node(t_lexems *list, char c);
void		add_str_node(t_lexems *list, char *str);
char		*list_to_string(t_lexems *list);
char		process_node(t_lexems *list);
void parse_expander_sign_in_quotes(int *i, char *line, t_lexems *list, t_args *args);

/* sofa */
void	write_new_promt(void);

/* builtins */
int	run_buildin(t_cmd_info	*ecmd, t_args *params,  t_cmd_info *cmd_list, int **pipe_arr);
int is_buildin(char *cmd);
int check_if_single_builtin(t_cmd_info *cmd);
int run_single_builtin(t_cmd_info *cmd, t_args *params, t_cmd_info *cmd_list, int **pipe_arr);

int cd_cmd(t_cmd_info *ecmd, t_args *params);
int echo_cmd(t_cmd_info *ecmd);
int pwd_cmd(t_cmd_info *ecmd, t_args *params);
int	env_cmd(t_cmd_info *ecmd, t_args *params);
int pre_export_cmd(t_cmd_info *ecmd, t_args *params);
int export_cmd(char *str, t_args *params);
int pre_unset_cmd(t_cmd_info *ecmd, t_args *params);
int	unset_cmd(char *str, t_args *params);
void	exit_cmd(t_cmd_info *ecmd, t_args *params,  t_cmd_info *cmd_list, int **pipe_arr);

char	*get_str_after_sign(char *str, char sign); //export
char	*get_str_before_sign(char *str, char sign); //export
int		add_cmd(t_args *params, char *new_env_var); //export
int		export_print(t_args *params); //export
int		remove_cmd(t_args *params, char *env_var_to_remove); //unset


/* env utils */
int		update_envp_var(t_args *params, char *env_var, char *new_content);
char	*find_env_var(char **envp, char *var);
int		find_env_index(char **envp, char *var);

/* redirections */
void	redir(t_redir *rcmd);
void	close_fd(t_cmd *ecmd);

int		get_file_fd(int fd, char *file, int mode);
int		define_fd(t_cmd_info	*rcmd, t_redir *old_cmd, t_args *args);
int		define_file(t_cmd_info	*rcmd, t_redir *old_cmd);
int		add_redir_details(t_cmd_info	*new_cmd, t_redir *rcmd, t_args *args);

char	*heredoc_get_tmp_file(void);
int		heredoc(int fd, char *file, char *limiter, int mode, t_args *args);
int		old_heredoc(t_redir *rcmd);

int		append(int fd, char **eargv, t_args *args);

/* expantion */
char	*add_expantion(char *input, t_args *args);
int		is_expantion(char *input);

/* list */
t_cmd_info	*create_cmdlist(t_cmd *cmd, t_args *args);
t_cmd_info	*fill_redir(t_cmd *cmd, t_cmd_info **cmd_list, t_args *args);
t_cmd_info	*fill_exec(t_cmd *cmd);
void	add_cmd_to_list(t_cmd_info *cmd, t_cmd_info	**head);
void	gothrough_cmd(t_cmd *cmd, t_cmd_info **cmd_list, t_args *args);
void	fill_pipe(t_cmd *cmd, t_cmd_info **cmd_list, t_args *args);
int		more_redir(t_cmd_info *new_cmd, t_redir *rcmd, t_args *args);
int		list_size(t_cmd_info *cmd_list);

void	check_arguments(t_cmd_info *ecmd);
void	copy_eargv(t_cmd_info *new_cmd, t_cmd *cmd);
void	copy_argv(t_cmd_info *new_cmd, t_cmd *cmd);
int		check_file_access(const char *file_path, int mode);

/* connections between cmds */
int		**connections(t_cmd_info *cmd_list);
int		**fill_pipes(t_cmd_info *cmd, int **pipe_arr, int i, int size);
int		*create_a_pipe(int **pipe_arr);
int		connection_content(t_cmd_info	*new_cmd);

/* running cmds */
int		run_cmds(t_cmd_info *cmd_list, int **pipe_arr, t_args *args);
int		execute_cmd(t_cmd_info *cmd, t_cmd_info *cmd_list, int **pipe_arr, t_args *params);
void		run_exec(t_cmd_info *cmd, t_cmd_info *cmd_list, int **pipe_arr, t_args *params);
int		wait_cmds(t_cmd_info *cmd_head);

/* free */
void	free_envp(t_args *args);
void	free_cmd_list(t_cmd_info	*cmd_list);
void	*close_free_pipe_arr(int **pipe_arr);
void	free_hfile_arr(char **hfile_array);
void    free_all(t_cmd_info	*cmd_list, int **pipe_arr);
void	free_and_exit(int status, t_cmd_info *cmd_list, int **pipe_arr, t_args *params);
void PrintTree(t_cmd	*cmd);
#endif