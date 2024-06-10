/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 17:05:31 by sabdulki          #+#    #+#             */
/*   Updated: 2024/06/10 20:02:29 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int update_envp_var(t_args *params, char *env_var, char *new_content)
{
	char	*full_var;
    int     index;

    index = find_env_index(params->envp, env_var);
	full_var = ft_strjoin(env_var, "=");
	full_var = ft_strjoin(full_var, new_content);	
	free(params->envp[index]);
    params->envp[index] = full_var;
    return (0);
}

int find_env_index(char **envp, char *var)
{
    int		i;
    int     len;

	i = 0;
    len = ft_strlen(var);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var, len) == 0)
            return (i);
		i++;
	}
    printf("No such env variable\n");
    return (1);
}

char    *find_env_var(char **envp, char *var)
{
	int		i;
    int     len;
	char	*res;

	i = 0;
    len = ft_strlen(var);
	res = NULL;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var, len) == 0)
		{
			res = envp[i] + (len + 1);
			// res = envp[i];
            return (res);
		}
		i++;
	}
    printf("No such env variable\n");
	return (NULL);
}
