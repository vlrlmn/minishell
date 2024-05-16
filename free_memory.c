/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_memory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:44:06 by vlomakin          #+#    #+#             */
/*   Updated: 2024/05/16 18:03:32 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
