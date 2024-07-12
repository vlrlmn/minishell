/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 17:05:13 by sabdulki          #+#    #+#             */
/*   Updated: 2024/07/12 16:41:32 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	check_argument_amount(t_cmd_info *ecmd, t_args *params,
		t_cmd_info *cmd_list, int **pipe_arr)
{
	int		arg_counter;
	char	*path;

	arg_counter = 0;
	while (ecmd->argv[arg_counter])
		arg_counter++;
	if (ecmd->argv[1])
	{
		path = get_env("PATH=", params->envp);
		if (is_buildin(ecmd->argv[1]) || find_command_path(ecmd->argv[1], path))
			free_and_exit(255, cmd_list, pipe_arr, params, NULL);
	}
	if (arg_counter > 2)
		free_and_exit(1, cmd_list, pipe_arr, params, NULL);
}

void	check_first_char(char *status, t_cmd_info *cmd_list, int **pipe_arr,
		t_args *params)
{
	int	minus_counter;
	int	plus_counter;
	int	i;

	i = 0;
	minus_counter = 0;
	plus_counter = 0;
	while (status[i])
	{
		if (ft_isalpha(status[i]) || minus_counter > 1 || plus_counter > 1)
			free_and_exit(255, cmd_list, pipe_arr, params, NULL);
		if (status[i] == '-')
			minus_counter++;
		if (status[i] == '+')
			plus_counter++;
		i++;
	}
}

void	exit_cmd(t_cmd_info *ecmd, t_args *params, t_cmd_info *cmd_list,
		int **pipe_arr)
{
	char	*status;
	int		num_st;

	num_st = 0;
	check_argument_amount(ecmd, params, cmd_list, pipe_arr);
	status = ecmd->argv[1];
	if (status)
	{
		check_first_char(status, cmd_list, pipe_arr, params);
		num_st = ft_atoi(status);
		if (num_st > 255 || num_st < 0)
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
	free_and_exit(num_st, cmd_list, pipe_arr, params, NULL);
}
