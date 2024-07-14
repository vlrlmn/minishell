/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_buildin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 14:53:53 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/07/12 17:10:26 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	run_buildin(t_cmd_info *ecmd, t_args *params, t_cmd_info *cmd_list,
		int **pipe_arr)
{
	int	status;

	status = -1;
	if (ft_strncmp(ecmd->argv[0], "cd", 2) == 0)
		status = cd_cmd(ecmd, params);
	else if (ft_strncmp(ecmd->argv[0], "exit", 4) == 0)
		exit_cmd(ecmd, params, cmd_list, pipe_arr);
	else if (ft_strncmp(ecmd->argv[0], "echo", 4) == 0)
		status = echo_cmd(ecmd);
	else if (ft_strncmp(ecmd->argv[0], "pwd", 3) == 0)
		status = pwd_cmd(ecmd, params);
	else if (ft_strncmp(ecmd->argv[0], "export", 6) == 0)
		status = pre_export_cmd(ecmd, params);
	else if (ft_strncmp(ecmd->argv[0], "env", 3) == 0)
		status = env_cmd(ecmd, params);
	else if (ft_strncmp(ecmd->argv[0], "unset", 5) == 0)
		status = pre_unset_cmd(ecmd, params);
	else
	{
		fprintf(stderr, "Command not found: %s\n", ecmd->argv[0]);
		return (127);
	}
	return (status);
}

int	is_buildin(char *cmd)
{
	int	len;

	if (!cmd)
		return (0);
	len = ft_strlen(cmd);
	return (ft_strncmp(cmd, "cd", len) == 0 || ft_strncmp(cmd, "exit", len) == 0
		|| ft_strncmp(cmd, "echo", len) == 0 || ft_strncmp(cmd, "pwd", len) == 0
		|| ft_strncmp(cmd, "export", len) == 0 || ft_strncmp(cmd, "env",
			len) == 0 || ft_strncmp(cmd, "unset", len) == 0);
}

int	check_if_single_builtin(t_cmd_info *cmd)
{
	if (cmd->argv[0] == 0)
		exit(127);
	return (cmd->type == EXEC && is_buildin(cmd->argv[0]));
}

int	run_single_builtin(t_cmd_info *cmd, t_args *params, t_cmd_info *cmd_list,
		int **pipe_arr)
{
	int	builtin_status;
	int	saved_stdout;

	if (cmd->redir_type == APPEND)
	{
		saved_stdout = dup(STDOUT_FILENO);
		if (saved_stdout < 0)
			return (perror("dup"), 1);
		if (dup2(cmd->fd_write, STDOUT_FILENO) < 0)
			return (close(saved_stdout), perror("dup2"), 1);
	}
	builtin_status = run_buildin(cmd, params, cmd_list, pipe_arr);
	if (cmd->redir_type == APPEND)
	{
		if (dup2(saved_stdout, STDOUT_FILENO) < 0)
			return (close(saved_stdout), perror("dup2"), 1);
		close(saved_stdout);
	}
	return (builtin_status);
}
