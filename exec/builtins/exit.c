/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 17:05:13 by sabdulki          #+#    #+#             */
/*   Updated: 2024/07/01 16:56:08 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	exit_cmd(t_cmd_info *ecmd, t_args *params, t_cmd_info *cmd_list, int **pipe_arr)
{
	int		i;
	int		arg_counter;
	int		minus_counter;
	int		plus_counter;
	char	*status;
	char	*path;
	int		num_st;
	char 	*cmd_path;
	
	i = 0;
	num_st = 0;
	arg_counter = 0;
	minus_counter = 0;
	plus_counter = 0;
	status = ecmd->argv[1];
	while (ecmd->argv[arg_counter])
		arg_counter++;
	if (ecmd->argv[2])
	{
		path = get_env("PATH=", params->envp);
		cmd_path = find_command_path(ecmd->argv[2], path);
		if (is_buildin(ecmd->argv[2]) || cmd_path)
		{
			free(cmd_path);
			free_and_exit(255, cmd_list, pipe_arr, params);
		}
		free(cmd_path);
	}
	if (arg_counter > 2)
	{
		// printf("exit: too many arguments\n");
		free_and_exit(1, cmd_list, pipe_arr, params);
	}
	if (status)
	{
		while(status[i])
		{
			if (ft_isalpha(status[i]) || minus_counter > 1 || plus_counter > 1)
				free_and_exit(255, cmd_list, pipe_arr, params);
			if (status[i] == '-')
				minus_counter++;
			if (status[i] == '+')
				plus_counter++;
				// printf("it's not digit: %c\n", status[i]);
					// printf("numeric argument required: %c\n", status[i]);
			i++;
		}
		num_st = ft_atoi(status);
		// printf("num: %d\n", num_st);
		if (num_st > 255 || num_st < 0) //overflow
		{
			if (num_st < 0)
			{
				num_st *= -1;
				num_st = num_st % 256;
				num_st = 256 - num_st;
				
			}
			else
				num_st = num_st % 256;
		}
	}
	// printf("num after manipulation: %d\n", num_st);
	free_and_exit(num_st, cmd_list, pipe_arr, params);
}

