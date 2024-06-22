
#include "../minishell.h"

int	**fill_pipes(t_cmd_info *cmd, int **pipe_arr, int i)
{
	int			*pfd;

	if (cmd->inout != 'o')
	{
		pfd = create_a_pipe(pipe_arr);
		if (!pfd)
			return (NULL);
		pipe_arr[i] = pfd;
	}
	if (cmd->inout == 'i')
	{
		cmd->connection[0] = cmd->file_fd;
		cmd->connection[1] = pfd[1];
	}
	if (cmd->inout == 'c')
	{
		cmd->connection[0] = pipe_arr[i - 1][0];
		cmd->connection[1] = pfd[1];
	}
	if (cmd->inout == 'o')
	{
		cmd->connection[0] = pipe_arr[i - 1][0];
		cmd->connection[1] = cmd->file_fd;
	}
	return (pipe_arr);
}

int	*create_a_pipe(int **pipe_arr)
{
	int			*pfd;

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

int	close_free_pipe_arr(int **pipe_arr)
{
	int	i;

	i = 0;
	while (pipe_arr[i])
	{
		close(pipe_arr[i][0]);
		close(pipe_arr[i][1]);
		free(pipe_arr[i]);
		i++;
	}
	free(pipe_arr);
	return (1);
}