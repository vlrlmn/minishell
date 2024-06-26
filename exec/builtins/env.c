/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 14:20:59 by sabdulki          #+#    #+#             */
/*   Updated: 2024/06/25 18:01:49 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int env_cmd(t_cmd_info *ecmd, t_args *params)
{
	int	i;
	char	*value;

	i = 0;
	(void)ecmd;
	while (params->envp[i])
	{
		//if value of variable is "", do not print it.
		//should print if value contain only spaces (CHECK IN BASH 3.2)
		value = get_str_after_sign(params->envp[i], '=');
		if (value[0] != '\0')
			printf("%d: %s\n", i, params->envp[i]);
		i++;
	}
	return (0);
}