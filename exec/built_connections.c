/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_connections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 15:21:12 by sabdulki          #+#    #+#             */
/*   Updated: 2024/07/14 09:34:50 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	set_one_cmd(int size, t_cmd_info *cmd_list)
{
	if (size == 1)
	{
		cmd_list->connection[0] = cmd_list->fd_read;
		cmd_list->connection[1] = cmd_list->fd_write;
		return (1);
	}
	return (0);
}

int	**connections(t_cmd_info *cmd_list)
{
	int			i;
	int			size;
	int			**pipe_arr;
	t_cmd_info	*cmd;

	size = list_size(cmd_list);
	if (set_one_cmd(size, cmd_list))
		return (NULL);
	pipe_arr = malloc(sizeof(int *) * (list_size(cmd_list)));
	if (!pipe_arr)
		return (NULL);
	i = 0;
	cmd = cmd_list;
	while (cmd)
	{
		pipe_arr = fill_pipes(cmd, pipe_arr, i, size);
		if (!pipe_arr)
			return (NULL);
		cmd = cmd->next;
		i++;
	}
	pipe_arr[i - 1] = NULL;
	return (pipe_arr);
}

void	set_first_cmd_pipe(t_cmd_info *cmd, int size, int *pfd)
{
	cmd->connection[0] = cmd->fd_read;
	if (!cmd->file_write && size > 1)
		cmd->connection[1] = pfd[1];
	else
		cmd->connection[1] = cmd->fd_write;
}

int	**fill_pipes(t_cmd_info *cmd, int **pipe_arr, int i, int size)
{
	int	*pfd;

	if (cmd->next != NULL)
	{
		pfd = create_a_pipe(pipe_arr);
		if (!pfd)
			return (NULL);
		pipe_arr[i] = pfd;
	}
	if (cmd->head == 1)
		set_first_cmd_pipe(cmd, size, pfd);
	else
	{
		if (!cmd->file_read)
			cmd->connection[0] = pipe_arr[i - 1][0];
		else
			cmd->connection[0] = cmd->fd_read;
		if (cmd->index == size || cmd->file_write != NULL)
			cmd->connection[1] = cmd->fd_write;
		else
			cmd->connection[1] = pfd[1];
	}
	return (pipe_arr);
}

int	*create_a_pipe(int **pipe_arr)
{
	int	*pfd;

	pfd = malloc(sizeof(int) * 2);
	if (!pfd)
	{
		close_free_pipe_arr(pipe_arr);
		return (NULL);
	}
	if (pipe(pfd) != 0)
	{
		free(pfd);
		close_free_pipe_arr(pipe_arr);
		return (NULL);
	}
	return (pfd);
}
