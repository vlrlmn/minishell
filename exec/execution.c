/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 14:51:13 by sabdulki          #+#    #+#             */
/*   Updated: 2024/06/22 21:57:32 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	run_cmds(t_cmd_info *cmd_list, int **pipe_arr, t_args *args)
{
	t_cmd_info	*cmd;
	int			status;

	cmd = cmd_list;
	while (cmd)
	{
		status = execute_cmd(cmd, cmd_list, pipe_arr, args);
		if (status != 0)
			return (exit_with_err("Command is not executed\n"), 1); //?
		cmd = cmd->next;
	}
	return (0);
}

int	execute_cmd(t_cmd_info *cmd, t_cmd_info *cmd_list, int **pipe_arr, t_args *params)
{
	int	builtin_status;

	if (cmd->argv[0] == 0)
		return (1); //error
	check_arguments(cmd);
	if (is_buildin(cmd->argv[0]))
	{
		builtin_status = run_single_builtin(cmd, params);
			return (builtin_status);
		fprintf(stderr, "STATUS %d\n", builtin_status);
	}
	else
	{	
		fprintf(stderr, "Executing command: %s\n", cmd->argv[0]); // Debug message
		if (run_exec(cmd, cmd_list, pipe_arr, params))
			return (1);
	}
	return (0);
}

int	if_path_to_cmd(char *path_line)
{
	if (access(path_line, F_OK) == -1)
		return (1);
	if (access(path_line, X_OK) == -1)
	{
		perror("cmd is not executable\n");
		return (1);
	}
	return (0);
}

int	run_exec(t_cmd_info *cmd, t_cmd_info *cmd_list, int **pipe_arr, t_args *params)
{
	char	*cmd_path;
	char	*path;
	int	pid;
	int	status;
	(void)cmd_list;
	(void)pipe_arr;

	cmd_path = NULL;
	pid = fork();
	if (pid < 0)
		return (1);
	if (pid == 0)
	{
		if (!params)
			fprintf(stderr, "!cmd->params && !cmd->params->envp\n");
		path = get_env("PATH=", params->envp);
		cmd_path = find_command_path(cmd->argv[0], path);
		if (!cmd_path)
		{
			return (fprintf(stderr, "Command not found: %s\n", cmd->argv[0]), 1);
		}
		if (if_path_to_cmd(cmd_path))
			return (1);
		fprintf(stderr, "Found the path! : %s\n", cmd_path);
		if (cmd->connection[0] != 0)
		{
			dup2(cmd->connection[0], STDIN_FILENO);
			close(cmd->connection[0]);
			fprintf(stderr, "did dup2 for con[0]!\n");
		}
		if (cmd->connection[1] != 1)
		{
			dup2(cmd->connection[1], STDOUT_FILENO);
			close(cmd->connection[1]);
			fprintf(stderr, "did dup2 for con[1]!\n");
		}
		status = execve(cmd_path, cmd->argv, params->envp);
		fprintf(stderr, "execve errno: %d\n", status);
		// free_cmd_list(cmd_list);
		// close_free_pipe_arr(pipe_arr);
		// exit(status);
		return (status);
	}
	else
	{
		if (cmd->connection[0] != 0)
			close(cmd->connection[0]);
		if (cmd->connection[1] != 1)
			close(cmd->connection[1]);
		return (0);
	}
}

int	wait_cmds(t_cmd_info *cmd_head)
{
	int	i;
	int	size;
	int	status;

	i = 0;
	size = list_size(cmd_head);
	while (i < size)
	{
		wait(&status);
		status = WEXITSTATUS(status);
		i++;
	}
	return (status);
}