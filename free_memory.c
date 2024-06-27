/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_memory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:44:06 by vlomakin          #+#    #+#             */
/*   Updated: 2024/06/27 20:04:47 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_split(char **arr) 
{
    int i = 0;
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
		fprintf(stderr, "envp: '%s'\n", args->envp[i]);
		free((void*)args->envp[i]);
		i++;
	}
	free(args->envp);
}

void    free_all(t_cmd_info	*cmd_list, int **pipe_arr, t_args *params)
{
	(void)params;
    free_cmd_list(cmd_list);
    close_free_pipe_arr(pipe_arr);
	// free_envp(params); committed becuse it should be frees only in exit or in loop_result!
	//not in exec();
}

void	free_and_exit(int status, t_cmd_info *cmd_list, int **pipe_arr, t_args *params)
{
	free_all(cmd_list, pipe_arr, params);
	exit(status);
}