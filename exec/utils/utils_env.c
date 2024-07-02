/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 17:05:31 by sabdulki          #+#    #+#             */
/*   Updated: 2024/07/02 19:53:37 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int update_envp_var(t_args *params, char *env_var, char *new_content)
{
	char	*full_var;
	char	*before_sign;
    int     index;

    index = find_env_index(params->envp, env_var);
	if (index == -1)
		return (1);
	before_sign = ft_strjoin(env_var, "=");
	full_var = ft_strjoin(before_sign, new_content);
	// free(before_sign);
	free(params->envp[index]);
    params->envp[index] = full_var;
    return (0);
}

char	*get_env(char *value, char **envp)
{
	int i;
    // fprintf(stderr, "get_env in \n");
	i = 0;
	while (envp[i])
	{
		// fprintf(stderr, "envp[%d]: %s \n",i, envp[i]);
		if (ft_strlen(value) && (ft_strncmp(value, envp[i], ft_strlen(value)) == 0))
			return (envp[i]);
		i++;
	}
	// fprintf(stderr, "get_env out \n");
	return (NULL);
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
    // printf("No such env variable\n");
    return (-1);
}

char    *find_env_var(char **envp, char *var)
{
	int		i;
	size_t	len;
	char	*res;
	char	*before_sign;

	i = 0;
	res = NULL;
	if (!var)
		return (NULL);
    len = ft_strlen(var);
	while (envp[i])
	{
		before_sign = get_str_before_sign(envp[i], '=');
		if (ft_strncmp(envp[i], var, len) == 0 && (ft_strlen(before_sign) == len))
		{
			res = envp[i] + (len + 1);
			free(before_sign);
			// maybe dould add substr to allocate memory for this substring
			// return (ft_strdup(res));
			return (res);
		}
		free(before_sign);
		i++;
	}
    // printf("No such env variable\n");
	return (NULL);
}

