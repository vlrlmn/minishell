/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 20:52:48 by sabdulki          #+#    #+#             */
/*   Updated: 2024/06/06 21:35:51 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	unset_cmd(t_execcmd *ecmd, t_args *params)
{
	if (ecmd->argv[1])
    {
        if (find_env_var(params->envp, ecmd->argv[1]))
            remove_cmd(params, ecmd->argv[1]);
    }
    else
    {
        printf("\tunset: invalid argument\n"); // free memory where necessary
        return(1);
    }
    return (0);
}

int	remove_cmd(t_args *params, char *env_var_to_remove)
{
	int size;
    int i;
	int	i_new;
	int	index;
    char **new_env_list;

    i = 0;
	i_new = 0;
    size = 0;
    while (params->envp[size])
        size++;
    new_env_list = malloc(sizeof(char *) * size);
    if (!new_env_list)
        return (1);
    while (i < size)
    {
		index = find_env_index(params->envp, env_var_to_remove);
		if (index == i)
			free(params->envp[i]);
        else
		{
			new_env_list[i_new] = params->envp[i];
			i_new++;
		}
        i++;
    }
    new_env_list[i_new] = NULL;
    free(params->envp);
    params->envp = new_env_list;
    return (0);
}