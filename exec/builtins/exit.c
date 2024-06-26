/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 17:05:13 by sabdulki          #+#    #+#             */
/*   Updated: 2024/06/25 17:33:14 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	exit_cmd(t_cmd_info *ecmd, t_args *params)
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
					//free all
					exit(2);
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
	free_envp(params);
	//free all memory, close fd-s, connections
	exit(num_st);
}

