/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 17:18:22 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/07/04 16:26:34 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	is_n_option(char *str)
{
	int	i;

	i = 1;
	if (str[0] == '-')
	{
		while (str[i] && str[i] == 'n')
			i++;
		if (str[i] == '\0')
			return (1);
	}
	return (0);
}

int echo_cmd(t_cmd_info *ecmd)
{
	int		i;
	int		n_flag;

	i = 1;
	n_flag = 1;
	while (ecmd->argv[i] && is_n_option(ecmd->argv[i]))
	{
		n_flag = 0;
		i++;
	}
	while (ecmd->argv[i])
	{
		printf("%s", ecmd->argv[i]);
		if (ecmd->argv[i++ + 1])
			printf(" ");
	}
	if (n_flag)
		printf("\n");
    return (0);
}
