/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 14:20:59 by sabdulki          #+#    #+#             */
/*   Updated: 2024/07/05 17:23:16 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int env_cmd(t_cmd_info *ecmd, t_args *params)
{
	int	i;
	char *value;
	char *name;

	i = 0;
	(void)ecmd;
	while (params->envp[i])
	{
		//if value of variable is "", do not print it.
		//should print if value contain only spaces (CHECK IN BASH 3.2)
		name = get_str_before_sign(params->envp[i], '=');
		value = get_str_after_sign(params->envp[i], '=');
		if (value == NULL || (ft_strncmp(name, "OLDPWD", ft_strlen(name)) == 0 && value[0] == '\0'))
		{
			free(name);
			free(value);
			i++;
			continue ;
		}
		printf("%s\n", params->envp[i]);
		free(name);
		free(value);
		i++;
	}
	return (0);
}