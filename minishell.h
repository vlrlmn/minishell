/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlomakin <vlomakin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 19:02:23 by vlomakin          #+#    #+#             */
/*   Updated: 2024/07/14 19:30:04 by vlomakin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "Libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <math.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <sysexits.h>
# include <termios.h>
# include <unistd.h>

# define MALLOC_ERROR 69
# define SYNTAX_ERR 2
# define MAXARGS 1024
# define Y "\033[1;33m"
# define RST "\033[0m"

extern int				g_sig_exit_status;

typedef struct s_args
{
	char				*input;
	char				**envp;
}						t_args;
typedef struct s_cmd
{
	int					type;
}						t_cmd;
typedef struct s_execmd
{
	int					type;
	char				*argv[MAXARGS];
	char				*eargv[MAXARGS];
}						t_execcmd;
typedef struct s_pipe
{
	int					type;
	char				*argv[MAXARGS];
	char				*eargv[MAXARGS];
	t_cmd				*left;
	t_cmd				*right;
}						t_pipe;
typedef struct s_redir
{
	int					type;
	int					subtype;
	char				*argv[MAXARGS];
	char				*eargv[MAXARGS];
	t_cmd				*cmd;
	char				*file;
	char				*efile;
	int					mode;
	int					fd;
}						t_redir;
typedef struct s_lexem_node
{
	void				*data;
	struct s_lexem_node	*next;
}						t_lexem_node;
typedef struct s_lexems
{
	t_lexem_node		*head;
	t_lexem_node		*tail;
}						t_lexems;
typedef enum quotes_handler
{
	QUOTES_ERR,
	MULTIPLE_QUOTES,
	NO_QUOTES,
	SINGLE_Q,
	DOUBLE_Q,
}						t_quotes;
typedef enum redir_type
{
	HEREDOC = 3,
	APPEND = 4,
	REDIRIN = 5,
	REDIROUT = 6,
	NONE = -1
}						t_rtype;

typedef enum token_type
{
	PIPE = 0,
	EXEC = 1,
	REDIR = 2,
}						t_type;

typedef enum signal_status
{
	IN_CMD = 0,
	IN_HEREDOC = 1,
	STOP_CMD = 2,
	STOP_HEREDOC = 3,
	GET = 4,
	SET = 5,
	SET_ZERO = 6,
	CTRL_D = 7,
}						t_signal_type;

typedef struct s_cmd_info
{
	t_args				*args;
	int					head;
	int					index;
	int					type;
	int					redir_type;
	t_cmd				*subcmd;
	char				*argv[MAXARGS];
	char				*eargv[MAXARGS];
	int					fd_read;
	int					fd_write;
	char				*file_read;
	char				*file_write;
	int					mode_read;
	int					mode_write;
	int					*connection;
	struct s_cmd_info	*next;
}						t_cmd_info;

typedef struct s_clean_line_args
{
	char				*line;
	t_lexems			*list;
	t_args				*args;
}						t_clean_line_args;

void					lexical_analysis(t_cmd *cmd, t_args *args,
							int *exit_status);
void					old_check_arguments(t_execcmd *ecmd);
void					parse_double_quote(int *i, t_clean_line_args args,
							int *exit_status);
void					parse_quote(char *line, int *i, t_lexems *list);
void					parse_expander_sign(int *i, char *line, t_lexems *list,
							t_args *args);
void					parse_expander(int *i, t_clean_line_args clean_args,
							int *exit_status);
char					*get_env(char *value, char **envp);
t_cmd					*nulterminate(t_cmd *cmd);
int						valid_input(char *work_line, int *exit_status);
void					free_split(char **arr);
void					run_cmd(t_cmd *cmd, t_args *params);
char					*find_cmd_path(char *cmd, char *path);
int						gettoken(char **ps, char *es, char **q, char **eq);
void					exit_with_err(char *msg);
int						peek(char **ps, char *es, char *toks);
t_cmd					*parseredir(t_cmd *cmd, char **ps, char *es);
t_cmd					*pipecmd(t_cmd *left, t_cmd *right);
t_cmd					*parsepipe(char **ps, char *es);
t_cmd					*parse(t_args *args, int *exit_status);
void					panic_and_free_env(t_args *args, int index);
void					exit_with_syntax_err(t_args *args, int err_code);
void					exit_with_malloc_error(int err_code);
int						is_delimiter(char c);
int						ft_isalnum(int c);
int						has_parse_symbol(char *s);
char					*clean_cmd(char *line, t_args *args, int *exit_status);
void					add_char_node(t_lexems *list, char c);
void					add_str_node(t_lexems *list, char *str);
char					*list_to_string(t_lexems *list);
char					process_node(t_lexems *list);
void					parse_expander_sign_in_quotes(int *i, char *line,
							t_lexems *list, t_args *args);
void					write_new_promt(void);
int						run_buildin(t_cmd_info *ecmd, t_args *params,
							t_cmd_info *cmd_list, int **pipe_arr);
int						is_buildin(char *cmd);
int						check_if_single_builtin(t_cmd_info *cmd);
int						run_single_builtin(t_cmd_info *cmd, t_args *params,
							t_cmd_info *cmd_list, int **pipe_arr);
int						cd_cmd(t_cmd_info *ecmd, t_args *params);
int						echo_cmd(t_cmd_info *ecmd);
int						pwd_cmd(t_cmd_info *ecmd, t_args *params);
int						env_cmd(t_cmd_info *ecmd, t_args *params);
int						pre_export_cmd(t_cmd_info *ecmd, t_args *params);
int						export_cmd(char *str, t_args *params);
int						pre_unset_cmd(t_cmd_info *ecmd, t_args *params);
int						unset_cmd(char *str, t_args *params);
void					exit_cmd(t_cmd_info *ecmd, t_args *params,
							t_cmd_info *cmd_list, int **pipe_arr);
int						is_executable(const char *path);
int						redir_tok_append(char **ps, char **q, char **eq,
							int *token);
int						redir_tok_heredoc(char **ps, char **q, char **eq,
							int *token);
char					*get_str_after_sign(char *str, char sign);
char					*get_str_before_sign(char *str, char sign);
int						add_cmd(t_args *params, char *new_env_var);
int						export_print(t_args *params);
int						check_var_name(char *env_var);
int						remove_cmd(t_args *params, char *env_var_to_remove);
int						is_valid_variable_name(char *key);
int						update_envp_var(t_args *params, char *env_var,
							char *new_content);
char					*find_env_var(char **envp, char *var);
int						find_env_index(char **envp, char *var);
void					redir(t_redir *rcmd);
void					close_fd(t_cmd *ecmd);
char					*get_file(t_cmd_info *cmd);
int						get_fd_or_mode(t_cmd_info *cmd, char flag);
int						get_file_fd(t_cmd_info *cmd, int redir_type);
int						r_get_file_fd(t_redir *rcmd, int subtype);
void					free_redir(t_redir *rcmd);
void					define_redir_info(t_cmd_info *new_cmd, t_redir *rcmd);
int						define_fd(t_cmd_info *rcmd, t_redir *old_cmd,
							t_args *args);
int						define_file(t_cmd_info *rcmd, t_redir *old_cmd);
int						add_redir_details(t_cmd_info *new_cmd, t_redir *rcmd,
							t_args *args);
int						count_files(t_redir *rcmd, int redir_type);
char					*heredoc_get_tmp_file(void);
int						heredoc(t_cmd_info *cmd, char *limiter, t_args *args);
int						call_heredocs(char **arr, t_cmd_info *new_cmd,
							t_args *args);
int						count_arr_elem(char **arr);
char					*free_heredoc_arr(char **arr, char flag);
int						check_input(char *input, int fd);
char					*do_expantion(char *input, t_cmd_info *cmd,
							t_args *args);
int						is_limiter(char *limiter, char *input);
char					*add_expantion(char *input, t_args *args);
int						is_expantion(char *input);
t_cmd_info				*create_cmdlist(t_cmd *cmd, t_args *args,
							int *exit_status);
t_cmd_info				*fill_redir(t_cmd *cmd, t_cmd_info **cmd_list,
							t_args *args, int *exit_status);
t_cmd_info				*fill_exec(t_cmd *cmd);
void					add_cmd_to_list(t_cmd_info *cmd, t_cmd_info **head);
void					gothrough_cmd(t_cmd *cmd, t_cmd_info **cmd_list,
							t_args *args, int *exit_status);
void					fill_pipe(t_cmd *cmd, t_cmd_info **cmd_list,
							t_args *args, int *exit_status);
int						more_redir(t_cmd_info *new_cmd, t_redir *rcmd,
							t_args *args);
int						list_size(t_cmd_info *cmd_list);
void					check_arguments(t_cmd_info *ecmd);
void					copy_eargv(t_cmd_info *new_cmd, t_cmd *cmd);
void					copy_argv(t_cmd_info *new_cmd, t_cmd *cmd);
int						check_file_access(const char *file_path,
							int redir_type);
int						**connections(t_cmd_info *cmd_list);
int						**fill_pipes(t_cmd_info *cmd, int **pipe_arr, int i,
							int size);
int						*create_a_pipe(int **pipe_arr);
int						connection_content(t_cmd_info *new_cmd);
int						run_cmds(t_cmd_info *cmd_list, int **pipe_arr,
							t_args *args);
int						execute_cmd(t_cmd_info *cmd, t_cmd_info *cmd_list,
							int **pipe_arr, t_args *params);
void					run_exec(t_cmd_info *cmd, t_cmd_info *cmd_list,
							int **pipe_arr, t_args *params);
int						wait_cmds(t_cmd_info *cmd_head);
void					free_envp(t_args *args);
void					free_cmd_list(t_cmd_info *cmd_list);
void					*close_free_pipe_arr(int **pipe_arr);
void					free_all(t_cmd_info *cmd_list, int **pipe_arr);
void					free_and_exit(int status, t_cmd_info *cmd_list,
							int **pipe_arr, char *cmd_path);
void					PrintTree(t_cmd *cmd);
char					*check_params(t_cmd_info *cmd, t_cmd_info *cmd_list,
							int **pipe_arr, t_args *params);
int						prepare_connections(t_cmd_info *cmd);
void					close_parent_connections(t_cmd_info *cmd);
int						get_status(void);
int						set_status(int new_status);
int						status_code(t_signal_type flag, int new_status);
void					handle_sigint(int sig);
void					handle_sigquit(int sig);
int						get_token(char **ps, char *es, char **q, char **eq);
int						validate_redirection(char **ps, char *es,
							int *exit_status);
int						check_invalid_pipe_syntax(char **ps, char *es,
							int *exit_status);
int						validate_pipe(char **ps, char *es, int *exit_status);
t_cmd					*parseexec(char **ps, char *es);
void					handle_quotes(char **ps, char *es);
void					skip_delimiters(char **ps, char *es);
void					print_content(t_cmd_info *current);
int						PrintList(t_cmd_info *cmd_list);
void					printPipeArr(int **pipe_arr);
char					**create_file_array(t_redir *rcmd, int redir_type);
int						fill_heredoc_array(char **heredoc_arr, int i,
							t_cmd_info *new_cmd, t_redir *rcmd);
void					free_files_and_fd(t_cmd_info *current);
void					free_argv(t_cmd_info *current);

#endif
