/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_connections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 15:21:12 by sabdulki          #+#    #+#             */
/*   Updated: 2024/06/28 19:02:21 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* there should be pipe_amount-1 amount of connections, 
and the last in pipe_ar should be NULL */
int	**connections(t_cmd_info *cmd_list)
{
	int			i;
	int			**pipe_arr;
	t_cmd_info	*cmd;
	int			size;

	size = list_size(cmd_list);
	// printf("size: %d\n", size);
	if (size < 2)
	{
		cmd_list->connection[0] = cmd_list->fd_read;
		cmd_list->connection[1] = cmd_list->fd_write;
		return (NULL);
	}
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
	// printf("------\n");
	if (cmd->head == 1) //for first cmd
	{
		// fprintf(stderr, "\ti'm head!\n");
		cmd->connection[0] = cmd->fd_read;
		if (!cmd->file_write && size > 1)
			cmd->connection[1] = pfd[1]; //pfd1[1]
		else
		{
			cmd->connection[1] = cmd->fd_write;
		}
	}
	else //other cmds
	{
		// fprintf(stderr, "\there!\n");
		if (!cmd->file_read)
			cmd->connection[0] = pipe_arr[i - 1][0]; //pfd1[0]
		else
			cmd->connection[0] = cmd->fd_read;
		if (cmd->index == size || cmd->file_write) //isn't last cmd
		{
			// fprintf(stderr, "\t2: my index: %d!\n", cmd->index);
			cmd->connection[1] = cmd->fd_write;
		}
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

/* void * allows to assign NULL to pipe_arr*/
void	*close_free_pipe_arr(int **pipe_arr)
{
	int	i;

	i = 0;
	if (!pipe_arr)
		return (NULL);
	while (pipe_arr[i])
	{
		// if (pipe_arr[i][0])
		if (pipe_arr[i][0] != 0 && pipe_arr[i][0] != 1)
			close(pipe_arr[i][0]);
		// if (pipe_arr[i][1])
		if (pipe_arr[i][1] != 0 && pipe_arr[i][1] != 1)
			close(pipe_arr[i][1]);
		free(pipe_arr[i]);
		i++;
	}
	free(pipe_arr);
	return (NULL);
}