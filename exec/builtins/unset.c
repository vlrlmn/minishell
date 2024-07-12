/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 20:52:48 by sabdulki          #+#    #+#             */
/*   Updated: 2024/07/12 18:16:34 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	pre_unset_cmd(t_cmd_info *ecmd, t_args *params)
{
	int	i;
	int	status;

	i = 1;
	while (ecmd->argv[i])
	{
		status = unset_cmd(ecmd->argv[i], params);
		i++;
	}
	return (status);
}

int	unset_cmd(char *str, t_args *params)
{
	char		*env_var;
	int			status;
	static int	print_counter;

	if (!str)
		return (printf("str: unset: invalid argument\n"), 1);
	print_counter += 1;
	env_var = get_env(str, params->envp);
	if (!env_var)
	{
		if (str[0] == '-' && ft_isalnum(str[1]))
		{
			printf("bash: unset: invalid option \n");
			return (2);
		}
		if (print_counter == 1)
			printf("bash: line 1: unset: not a valid identifier\n");
		return (1);
	}
	status = remove_cmd(params, env_var);
	return (status);
}

char	**fill_envlist(int len, char **envlist, char *remove, t_args *params)
{
	int		i;
	int		i_new;
	int		index;

	i = 0;
	i_new = 0;
	while (i < len)
	{
		index = find_env_index(params->envp, remove);
		if (index == i)
			free(params->envp[i]);
		else
		{
			envlist[i_new] = params->envp[i];
			i_new++;
		}
		i++;
	}
	envlist[i_new] = NULL;
	return (envlist);
}

int	remove_cmd(t_args *params, char *env_var_to_remove)
{
	int		size;
	char	**new_envlist;

	size = 0;
	if (!env_var_to_remove)
		return (1);
	while (params->envp[size])
		size++;
	new_envlist = malloc(sizeof(char *) * size);
	if (!new_envlist)
		return (1);
	new_envlist = fill_envlist(size, new_envlist, env_var_to_remove, params);
	free(params->envp);
	params->envp = new_envlist;
	return (0);
}
