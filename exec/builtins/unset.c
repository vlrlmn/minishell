/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 20:52:48 by sabdulki          #+#    #+#             */
/*   Updated: 2024/06/26 16:32:37 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int pre_unset_cmd(t_cmd_info *ecmd, t_args *params)
{
    int     i;

    i = 1;
    while(ecmd->argv[i])
    {
        unset_cmd(ecmd->argv[i], params);
        i++;
    }
    return (0);
}

int	unset_cmd(char *str, t_args *params)
{
    char    *env_var;

	if (!str)
        return (printf("unset: invalid argument\n"), 1);
    env_var = get_env(str, params->envp);
    if (remove_cmd(params, env_var))
        return (printf("\tunset: invalid argument\n"), 1); // free memory where necessary
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
    if (!env_var_to_remove)
        return (1);
    // fprintf(stderr, "var: %s\n", env_var_to_remove);
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