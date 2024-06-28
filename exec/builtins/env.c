/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 14:20:59 by sabdulki          #+#    #+#             */
/*   Updated: 2024/06/28 17:01:02 by sabdulki         ###   ########.fr       */
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
		if ((ft_strncmp(name, "OLDPWD", ft_strlen(name)) == 0 && value[0] == '\0') || value == NULL)
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