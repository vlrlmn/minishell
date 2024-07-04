/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 14:51:13 by sabdulki          #+#    #+#             */
/*   Updated: 2024/07/04 18:18:29 by sabdulki         ###   ########.fr       */
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
			return (exit_with_err("Command is not executed\n"), status);
		cmd = cmd->next;
	}
	return (0);
}

int	execute_cmd(t_cmd_info *cmd, t_cmd_info *cmd_list, int **pipe_arr, t_args *params)
{
	int	status;

	status = 0; // or another value?
	if (!cmd->argv[0] || cmd->argv[0][0] == '\0')
	{
		fprintf(stderr, "Cmd is not valid! It's NULL!\n");
		return (0); //error
	}
	check_arguments(cmd);
	if (is_buildin(cmd->argv[0]) && list_size(cmd_list) == 1)
	{
		fprintf(stderr, "Executing BUILTIN command: %s\n", cmd->argv[0]); // Debug message
		status = run_single_builtin(cmd, params, cmd_list, pipe_arr);
	}
	else
	{	
		fprintf(stderr, "Executing command: %s\n", cmd->argv[0]); // Debug message
		run_exec(cmd, cmd_list, pipe_arr, params);
	}
	return (status);
}

int	if_path_to_cmd(char *path_line)
{
	if (access(path_line, F_OK) == 0 && access(path_line, X_OK) == 0)
	{
		fprintf(stderr, "everything is ok!\n");
		return (0);
	}
	return (1);
}

int is_executable(const char *path) {
    struct stat sb;

    // Check if the path exists and is a regular file
    if (stat(path, &sb) == 0 && S_ISREG(sb.st_mode)) {
        // Check if the file is executable
        if (access(path, X_OK) == 0) {
            return (0); // The path is an executable command
        }
    }
    return 1; // The path is not an executable command
}

void	run_exec(t_cmd_info *cmd, t_cmd_info *cmd_list, int **pipe_arr, t_args *params)
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
		return (free_all(cmd_list, pipe_arr));
	/* if cmd->argv[0] != executable cmd, != built, but it is 
	value of a var in env list, just print it */
	if (pid == 0)
	{
		fprintf(stderr, "cmd->argv[0]: '%s'\n", cmd->argv[0]);
		if (!params)
			fprintf(stderr, "!cmd->params && !cmd->params->envp\n"); // exit??
		if (cmd->argv[0][0] == '/')
			cmd_path = cmd->argv[0];
		else if (!is_buildin(cmd->argv[0]))
		{
			path = get_env("PATH=", params->envp);
			cmd_path = find_command_path(cmd->argv[0], path);
			if (!cmd_path)
			{
				free(cmd_path);
				fprintf(stderr, "command not found: %s\n", cmd->argv[0]);
				free_and_exit(127, cmd_list, pipe_arr, params);
			}
		}
		if (!is_buildin(cmd->argv[0]))
		{
			if (is_executable(cmd_path))
			{
				fprintf(stderr, "%s: is a directory\n", cmd_path);
				free(cmd_path);
				free_and_exit(127, cmd_list, pipe_arr, params); //is it 1 in bash?
			}
		}
		// if (if_path_to_cmd(cmd_path) || !is_buildin(cmd->argv[0]))
		// {
		// 	free(cmd_path);
		// 	free_and_exit(127, cmd_list, pipe_arr, params); //is it 1 in bash?
		// }
		fprintf(stderr, "Found the path! : %s\n", cmd_path);
		if (cmd->connection[0] == -1 || cmd->connection[1] == -1)
			free_and_exit(1, cmd_list, pipe_arr, params);
		if (dup2(cmd->connection[0], STDIN_FILENO) == -1)
		{
			close(cmd->connection[0]);
			close(cmd->connection[1]);
			perror("dup2");
        	free_and_exit(EXIT_FAILURE, cmd_list, pipe_arr, params);
		}
			fprintf(stderr, "did dup2 for con[0]!\n");
		if (cmd->connection[0] != 0)
		{
			close(cmd->connection[0]);
			fprintf(stderr, "closed %d fd !\n", cmd->connection[0]);
		}
		if (dup2(cmd->connection[1], STDOUT_FILENO) == -1)
		{
			close(cmd->connection[1]);
			perror("dup2");
        	free_and_exit(EXIT_FAILURE, cmd_list, pipe_arr, params);
		}
			fprintf(stderr, "did dup2 for con[1]!\n");
		if (cmd->connection[1] != 1)
		{
			close(cmd->connection[1]);
			fprintf(stderr, "closed %d fd !\n", cmd->connection[1]);
		}
		pipe_arr = close_free_pipe_arr(pipe_arr);
		if (is_buildin(cmd->argv[0]))
		{
			status = run_buildin(cmd, params, cmd_list, pipe_arr);
		}
		else
			status = execve(cmd_path, cmd->argv, params->envp);
		fprintf(stderr, "execve errno: %d\n", status);
		free_and_exit(status, cmd_list, pipe_arr, params);
	}
	else
	{
		if (cmd->connection[0] != 0)
			close(cmd->connection[0]);
		if (cmd->connection[1] != 1)
			close(cmd->connection[1]);
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