/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aajaanan <aajaanan@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/01 20:04:09 by aajaanan          #+#    #+#             */
/*   Updated: 2023/09/01 20:04:30 by aajaanan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/utility.h"

void	free1(void *ptr)
{
	if (ptr)
		free(ptr);
	ptr = NULL;
}

void	free_split(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
	array = NULL;
}

void	free_exit(t_params *params, int exit_status)
{
	free1(params->buf);
	free_tree(params->tree);
	free_env_var_list(params->env_var_list);
	free_queue(&params->args_queue);
	exit(exit_status);
}

void	free_panic_exit(t_params *params, char *error, int exit_status)
{
	free1(params->buf);
	free_tree(params->tree);
	free_env_var_list(params->env_var_list);
	free_queue(&params->args_queue);
	perror(error);
	exit(exit_status);
}
