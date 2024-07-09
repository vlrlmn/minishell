/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 17:18:22 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/07/09 20:37:50 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//maybe we need to set static int to 0 afetr echo execution?
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

int	echo_cmd(t_cmd_info *ecmd)
{
	int	i;
	int	n_flag;

	i = 1;
	n_flag = 1;
	while (ecmd->argv[i] && is_n_option(ecmd->argv[i]))
	{
		n_flag = 0;
		i++;
	}
	while (ecmd->argv[i])
	{
		if (ft_strlen(ecmd->argv[i]) != 2 || ecmd->argv[i][0] != '\''
			|| ecmd->argv[i][1] != '\'')
			printf("%s", ecmd->argv[i]);
		if (ecmd->argv[i++ + 1])
			printf(" ");
	}
	if (n_flag)
		printf("\n");
	return (0);
}
