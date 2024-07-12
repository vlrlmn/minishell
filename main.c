/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:44:21 by vlomakin          #+#    #+#             */
/*   Updated: 2024/07/12 19:17:19 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_exit_status = 0;
// int	interrupted = 0;

void	write_new_promt(void)
{
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	print_content(t_cmd_info *current)
{
	int i;
	printf("\tINDEX: %d\n", current->index);
	printf("type: %d\n", current->type);
	i = 0;
	if (!current->argv[i])
		printf("NO ARGV!\n");
	while (current->argv[i] != NULL)
	{
		printf("ARGV %s\n", current->argv[i]);
		printf("EARGV %s\n", current->eargv[i]);
		i++;
	}
	printf("redir_type: %d\n", current->redir_type);
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

	pipe_arr = NULL;
	cmd_list = create_cmdlist(cmd, args);
	// printf("status: %d\n", status_code(GET, -1));
	if (status_code(GET, -1) == CTRL_D)
		g_exit_status = 0;
	if (!cmd_list)// or return g_exit_status, which i need to define in case of failure inside the create_cmdlist()
		return (free_all(cmd_list, pipe_arr), g_exit_status); 
	pipe_arr = connections(cmd_list);
	// PrintList(cmd_list);
	// printPipeArr(pipe_arr);
	exit_status = run_cmds(cmd_list, pipe_arr, args);
	if (!cmd_list->argv[0] || cmd_list->argv[0][0] == '\0')
		return (free_all(cmd_list, pipe_arr), exit_status);
	if (list_size(cmd_list) == 1 && is_buildin(cmd_list->argv[0]))
		return (free_all(cmd_list, pipe_arr), exit_status);
	exit_status = wait_cmds(cmd_list);
	free_all(cmd_list, pipe_arr);
	return (exit_status);
}

int	loop_result(t_args *args)
{
	t_cmd	*cmd;

	while (1)
	{
		args->input = readline("minishell$ ");
		if (args->input == NULL)
			break ;
		add_history(args->input);
		if (!valid_input(args->input))
			continue ;
		cmd = parse(args);
		g_exit_status = exec(cmd, args);
	}
	return (g_exit_status);
}

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
	export_cmd("OLDPWD=", args);
}

int	main(int argc, char **argv, char **envp)
{
	t_args	shell_context;
	int		exit_status;

	(void)argc;
	(void)argv;
	set_environment(&shell_context, envp);
	signal(SIGQUIT, handle_sigquit);
	signal(SIGINT, handle_sigint);
	exit_status = loop_result(&shell_context);
	clear_history();
	return (exit_status);
}
