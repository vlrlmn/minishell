/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 15:48:13 by vlomakin          #+#    #+#             */
/*   Updated: 2024/07/02 12:46:07 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	valid_quotes(char *input)
{
	int	count_single;
	int	count_double;
	int	i;

	i = 0;
	count_double = 0;
	count_single = 0;
	while (input[i])
	{
		if (input[i] == 34)
			count_double++;
		if (input[i] == 39)
			count_single++;
		i++;
	}
	if (count_single % 2 != 0 || count_double % 2 != 0)
		return (0);
	else
		return (1);
}

int	valid_input(char *work_line)
{
	int	i;

	i = 0;
	if (ft_strlen(work_line) == 0)
		return(0);
	while (is_delimiter(work_line[i]))
		i++;
	if (!work_line[i])
		return (0);
	if (!valid_quotes(work_line))
	{
		printf("Close quotes\n"); //TO_DO WRITE EXIT FROM  
		return (0);
	}
	return (1);
}
