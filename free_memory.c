/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_memory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:44:06 by vlomakin          #+#    #+#             */
/*   Updated: 2024/07/12 17:06:18 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* void * allows to assign NULL to pipe_arr*/
void	*close_free_pipe_arr(int **pipe_arr)
{
	int	i;

	i = 0;
	if (!pipe_arr)
		return (NULL);
	while (pipe_arr[i])
	{
		if (pipe_arr[i][0] != 0 && pipe_arr[i][0] != 1)
			close(pipe_arr[i][0]);
		if (pipe_arr[i][1] != 0 && pipe_arr[i][1] != 1)
			close(pipe_arr[i][1]);
		free(pipe_arr[i]);
		i++;
	}
	free(pipe_arr);
	return (NULL);
}

void	free_split(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	free_envp(t_args *args)
{
	int	i;

	i = 0;
	if (!args->envp)
		return ;
	while (args->envp[i])
	{
		free((void *)args->envp[i]);
		i++;
	}
	free(args->envp);
}

void	free_all(t_cmd_info *cmd_list, int **pipe_arr)
{
	free_cmd_list(cmd_list);
	close_free_pipe_arr(pipe_arr);
}

void	free_and_exit(int status, t_cmd_info *cmd_list, int **pipe_arr,
		t_args *params, char *cmd_path)
{
	if (cmd_path)
		free(cmd_path);
	free_all(cmd_list, pipe_arr);
	(void)params;
	exit(status);
}