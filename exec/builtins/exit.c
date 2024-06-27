/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 17:05:13 by sabdulki          #+#    #+#             */
/*   Updated: 2024/06/27 19:58:21 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	exit_cmd(t_cmd_info *ecmd, t_args *params, t_cmd_info *cmd_list, int **pipe_arr)
{
	int	i;
	char	*status;
	int		num_st;

	i = 0;
	num_st = 0;
	status = ecmd->argv[1];
	printf("in exit!\n");
	if (status)
	{
		while(status[i])
		{
			if (!ft_isdigit(status[i]))
			{
				if (status[i] != '-')
				{
					printf("numeric argument required\n");
					free_and_exit(2, cmd_list, pipe_arr, params);
				}
			}
			i++;
		}
		num_st = ft_atoi(status);
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
	free_and_exit(num_st, cmd_list, pipe_arr, params);
}

