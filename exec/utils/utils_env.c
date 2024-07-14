/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 17:05:31 by sabdulki          #+#    #+#             */
/*   Updated: 2024/07/12 18:17:54 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	update_envp_var(t_args *params, char *env_var, char *new_content)
{
	char	*full_var;
	char	*before_sign;
	int		index;

	index = find_env_index(params->envp, env_var);
	if (index == -1)
		return (1);
	before_sign = ft_strjoin(env_var, "=");
	full_var = ft_strjoin(env_var, new_content);
	free(before_sign);
	free(params->envp[index]);
	params->envp[index] = full_var;
	return (0);
}

char	*get_env(char *value, char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strlen(value) && (ft_strncmp(value, envp[i],
					ft_strlen(value)) == 0))
			return (envp[i]);
		i++;
	}
	return (NULL);
}

int	find_env_index(char **envp, char *var)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(var);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var, len) == 0)
			return (i);
		i++;
	}
	return (-1);
}

char	*find_env_var(char **envp, char *var)
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
		if (ft_strncmp(envp[i], var, len) == 0
			&& (ft_strlen(before_sign) == len))
		{
			res = envp[i] + (len + 1);
			return (free(before_sign), res);
		}
		free(before_sign);
		i++;
	}
	return (NULL);
}
