/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 14:51:13 by sabdulki          #+#    #+#             */
/*   Updated: 2024/07/13 20:41:05 by sabdulki         ###   ########.fr       */
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
			return (status);
		cmd = cmd->next;
	}
	return (0);
}

int	execute_cmd(t_cmd_info *cmd, t_cmd_info *cmd_list, int **pipe_arr,
		t_args *params)
{
	int	status;
	int	i;

	i = 0;
	status = 0;
	if (cmd->fd_read == -2)
		return (0);
	if (!cmd->argv[0] || cmd->argv[0][0] == '\0') // attention!
		return (1);
	check_arguments(cmd);
	while (cmd->argv[0][i])
	{
		*(cmd->argv[0] + i) = ft_tolower(cmd->argv[0][i]);
		i++;
	}
	status_code(SET, IN_CMD);
	if (is_buildin(cmd->argv[0]) && list_size(cmd_list) == 1)
		status = run_single_builtin(cmd, params, cmd_list, pipe_arr);
	else
		run_exec(cmd, cmd_list, pipe_arr, params);
	return (status);
}

void	run_exec(t_cmd_info *cmd, t_cmd_info *cmd_list, int **pipe_arr,
		t_args *params)
{
	char	*cmd_path;
	int		pid;
	int		status;

	pid = fork();
	if (pid < 0)
		return (free_all(cmd_list, pipe_arr));
	if (pid == 0)
	{
		cmd_path = check_params(cmd, cmd_list, pipe_arr, params);
		if (prepare_connections(cmd))
			free_and_exit(EXIT_FAILURE, cmd_list, pipe_arr, params, cmd_path);
		pipe_arr = close_free_pipe_arr(pipe_arr);
		if (is_buildin(cmd->argv[0]))
			status = run_buildin(cmd, params, cmd_list, pipe_arr);
		else
			status = execve(cmd_path, cmd->argv, params->envp);
		free_and_exit(status, cmd_list, pipe_arr, params, cmd_path);
	}
	else
		close_parent_connections(cmd);
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
		if (status_code(GET, -1) == STOP_CMD)
		{
			return (130);
		}
		i++;
	}
	return (status);
}
