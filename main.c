/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:44:21 by vlomakin          #+#    #+#             */
/*   Updated: 2024/07/03 17:35:59 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_exit_status = 0;

void	handle_sigint(int sig)
{
	if (sig == SIGINT)
	{
		write(STDERR_FILENO, "\n", 1);
		write_new_promt();
	}
}

void	write_new_promt(void)
{
	// rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void PrintTree(t_cmd	*cmd)
{
	t_execcmd *exec;
	t_pipe *pipe;
	t_redir *redir;

	int i = 0;

	if (cmd->type == EXEC)
	{
		exec = (t_execcmd*) cmd;
		i=0;
		while (exec->argv[i])
		{
			
			// printf("Arg %d: %.*s\n", i, (int)(exec->eargv[i] - exec->argv[i]), exec->argv[i]);
			// printf("ARGV %s\n", exec->argv[i]);
			// printf("EARGV %s\n", exec->eargv[i]);
			i++;
		}
	}
	else if (cmd->type == PIPE)
	{
		pipe = (t_pipe*) cmd;
		PrintTree(pipe->left);
		PrintTree(pipe->right);
	}
	else if (cmd->type == REDIR)
	{
		redir = (t_redir*)cmd;
		// printf("\nsubtype '%d'\n", redir->subtype);
		// printf("redir mode '%d'\n", redir->mode);
		// printf("redir file '%s'\n", redir->file);
		// printf("redir efile '%s'\n", redir->efile);
		PrintTree(redir->cmd);
		
	}
	else
	{
		printf("Unknown Type\n");
	}
}

int ft_launch_minishell(t_args *args)
{
	t_cmd	*cmd;
	

    if (args->input == NULL)
	{
		write(STDOUT_FILENO, "exit in launch\n", 5);
		return 1;
	}
	if (!valid_input(args->input))
	{
		free_envp(args);
		exit(SYNTAX_ERR);
	}
	//if (fork1() == 0)
	//{
		cmd = parse(args);
		PrintTree(cmd);
		run_cmd(cmd, args);
	//}
	//wait (0);
	return (0);
}

void	print_content(t_cmd_info *current)
{
	int i;
	printf("type: %d\n", current->type);
	i = 0;
	while (current->argv[i] != NULL)
	{
		printf("ARGV %s\n", current->argv[i]);
		printf("EARGV %s\n", current->eargv[i]);
		i++;
	}
	printf("con[0] %d\n", current->connection[0]);
	printf("con[1] %d\n", current->connection[1]);
	printf("fd_read: %d\n", current->fd_read);
	printf("file_read: %s\n", current->file_read);
	printf("fd_write: %d\n", current->fd_write);
	printf("file_write: %s\n", current->file_write);
}

int PrintList(t_cmd_info *cmd_list)
{
    t_cmd_info *current = cmd_list;
	printf("-------------PRINTING LIST-------------\n");
	if (!current)
		return (printf("no list nodes, error\n"), 1);
    while (current->next != NULL) 
	{
        print_content(current);
		if (current->next != NULL)
        	current = current->next;
		else
			return (printf("ERROR IN PRINTING\n"), 1);
    }
	if (current->next == NULL)
		print_content(current);
	printf("--------------------------------------\n");
	return (0);
}

void	printPipeArr(int **pipe_arr)
{
	int	i;

	i = 0;
	printf("going to print pipe arr\n");
	if (!pipe_arr)
		return ;
	while (pipe_arr[i])
	{
		fprintf(stderr, "pipe_arr[%d][0] = %d\n", i, pipe_arr[i][0]);
		fprintf(stderr, "pipe_arr[%d][1] = %d\n\n", i, pipe_arr[i][1]);
		i++;
	}
}

int	exec(t_cmd	*cmd, t_args *args)
{
	t_cmd_info	*cmd_list;
	int			**pipe_arr;
	int			exit_status;
	// int			status;
	// int			cmd_status;

	pipe_arr = NULL;
	cmd_list = create_cmdlist(cmd, args);
	pipe_arr = connections(cmd_list);
	// PrintList(cmd_list);
	// printPipeArr(pipe_arr);
	// exit_status = run_cmds(cmd_list, pipe_arr, args);
	// PrintList(cmd_list);
	exit_status = run_cmds(cmd_list, pipe_arr, args);
	if (list_size(cmd_list) == 1 && is_buildin(cmd_list->argv[0]))
		return (free_all(cmd_list, pipe_arr), exit_status);
	exit_status = wait_cmds(cmd_list);
	free_all(cmd_list, pipe_arr);
	return (exit_status);
}

/*This is where we have instant loop happening. Inside the loop
we reading the line, adding it in history and call lexer, beginning
of args->input parsing*/
void	free_args(void *ptr)
{
	if (ptr)
		free(ptr);
	ptr = NULL;
}

int	loop_result(t_args *args)
{
	t_cmd	*cmd;
	// int		status;

	while (1)
	{
		// printf(Y"NEW_PROMT:"RST);
		args->input = readline("minishell$ ");
		if (args->input == NULL)
		{
			// write(STDOUT_FILENO, "exit in loop\n", 14);
			// write(STDOUT_FILENO, "exit in loop\n", 5);
			break ;
		}
		if (!valid_input(args->input))
		{
			continue ;
		}
		add_history(args->input);
		cmd = parse(args);
		
		// printf("-------------END OF PARSING-------------\n");
		// status = exec(cmd, args);
		g_exit_status = exec(cmd, args);
		// printf("\tSTATUS: %d\n", g_exit_status);
		// printf("\tSTATUS: %d\n", status);
	}
	return (g_exit_status);
}

/*We need to create new environment argument and copy envp from main
arguments because this is safe way to work with environment*/
void	set_environment(t_args *args, char **envp)
{
	int	len;
	int	i;

	len = 0;
	i = 0;
	args->envp = NULL;
	while (envp[len])
		len++;
	args->envp = (char **)malloc((len + 1) * sizeof(char *));
	if (!args->envp)
		exit_with_malloc_error(MALLOC_ERROR);
	while (i < len)
	{
		args->envp[i] = ft_strdup(envp[i]);
		if (!args->envp[i])
			panic_and_free_env(args, i);
		i++;
	}
	args->envp[len] = NULL;
	export_cmd("OLDPWD", args);
	// unset_cmd("OLDPWD", args);
}

/*Here we launch our program, set environment, handle signals.
For saving history and mowing with arrows through
previously written commands we use	rl_clear_history(void);
In the end we clear everything and free memory*/
int	main(int argc, char **argv, char **envp)
{
	t_args	shell_context;
	int		exit_status;

	(void)argc;
	(void)argv;
	set_environment(&shell_context, envp);
	if (argc >= 3 && !ft_strncmp(argv[1], "-c", 3))
	{
		shell_context.input = argv[2];
		exit_status = ft_launch_minishell(&shell_context);
		return (exit_status);
	}
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	exit_status = loop_result(&shell_context);
	// rl_clear_history(); //idk why mac argue for it
	// rl_clear_history();
	clear_history();
	// free_envp (&shell_context);
	return (exit_status);
}

/*
about signals
1) should ctrl+c clear all memory or returning promt is enough? should there be ^C after pressing this cmd?
2) if 'return' pressed, should it quit? errno = 2
3) ctrl D -> "exit" str in the promt, not on the new line
*/