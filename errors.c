/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:44:03 by vlomakin          #+#    #+#             */
/*   Updated: 2024/05/13 18:08:07 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_structs(void) //TO_DO
{
}

void free_envp(t_args *args)
{
	int i;

	i = 0;
	while(args->envp[i])
	{
		free(args->envp[i]);
		i++;
	}
	free(args->envp);	
}

void	exit_with_syntax_err(t_args *args, int err_code)
{
	free_envp(args);
	write(2, "Quotes err\n", 12);
	exit(err_code);
}

void	exit_with_malloc_error(int err_code)
{
	write(2, "Malloc err\n", 12);
	exit(err_code);
}

void panic_and_free_env(t_args *args, int index)
{
	int i;

	i = 0;
	while(i < index)
	{
		free(args->envp[i]);
		i++;
	}
	free(args->envp);
	exit(MALLOC_ERROR);
}