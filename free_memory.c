/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlomakin <vlomakin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:44:06 by vlomakin          #+#    #+#             */
/*   Updated: 2024/05/15 12:30:08 by vlomakin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_envp(t_args *args)
{
	int	i;

	i = 0;
	while (args->envp[i])
	{
		free(args->envp[i]);
		i++;
	}
	free(args->envp);
}
