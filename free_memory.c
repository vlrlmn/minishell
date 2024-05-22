/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_memory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:44:06 by vlomakin          #+#    #+#             */
/*   Updated: 2024/05/22 14:53:08 by lomakinaval      ###   ########.fr       */
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
	while (args->envp[i])
	{
		free((void*)args->envp[i]);
		i++;
	}
	free(args->envp);
}
