/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlomakin <vlomakin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 22:07:48 by sabdulki          #+#    #+#             */
/*   Updated: 2024/07/14 16:48:24 by vlomakin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*check_params(t_cmd_info *cmd, t_cmd_info *cmd_list, int **pipe_arr,
		t_args *params)
{
	char	*cmd_path;

	cmd_path = NULL;
	if (!params)
		free_and_exit(1, cmd_list, pipe_arr, cmd_path);
	if (cmd->argv[0][0] == '/')
		cmd_path = cmd->argv[0];
	else if (!is_buildin(cmd->argv[0]))
	{
		cmd_path = find_cmd_path(cmd->argv[0], get_env("PATH=", params->envp));
		if (!cmd_path)
		{
			fprintf(stderr, "command not found: %s\n", cmd->argv[0]);
			free_and_exit(127, cmd_list, pipe_arr, cmd_path);
		}
	}
	if (!is_buildin(cmd->argv[0]))
	{
		if (is_executable(cmd_path))
		{
			fprintf(stderr, "%s: is a directory\n", cmd_path);
			free_and_exit(126, cmd_list, pipe_arr, NULL);
		}
	}
	return (cmd_path);
}

int	prepare_connections(t_cmd_info *cmd)
{
	if (cmd->connection[0] == -1 || cmd->connection[1] == -1)
		return (1);
	if (dup2(cmd->connection[0], STDIN_FILENO) == -1)
	{
		close(cmd->connection[0]);
		close(cmd->connection[1]);
		perror("dup2");
		return (1);
	}
	if (cmd->connection[0] != 0)
		close(cmd->connection[0]);
	if (dup2(cmd->connection[1], STDOUT_FILENO) == -1)
	{
		close(cmd->connection[1]);
		perror("dup2");
		return (1);
	}
	if (cmd->connection[1] != 1)
		close(cmd->connection[1]);
	return (0);
}

void	close_parent_connections(t_cmd_info *cmd)
{
	if (cmd->connection[0] != 0)
		close(cmd->connection[0]);
	if (cmd->connection[1] != 1)
		close(cmd->connection[1]);
}
