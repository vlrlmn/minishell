/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 14:20:59 by sabdulki          #+#    #+#             */
/*   Updated: 2024/06/06 14:22:39 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int env_cmd(t_execcmd *ecmd, t_args *params)
{
	(void)ecmd;

	int	i;

	i = 0;
	while(params->envp[i])
	{
		printf("%s\n", params->envp[i]);
		i++;
	}
	return (0);
}