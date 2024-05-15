/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlomakin <vlomakin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:44:03 by vlomakin          #+#    #+#             */
/*   Updated: 2024/05/15 12:33:39 by vlomakin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	panic_and_free_env(t_args *args, int index)
{
	int	i;

	i = 0;
	while (i < index)
	{
		free(args->envp[i]);
		i++;
	}
	free(args->envp);
	exit(MALLOC_ERROR);
}

void	exit_with_err(char *msg)
{
	write(2, "Too many args", 14);
}