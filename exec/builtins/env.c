/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 14:20:59 by sabdulki          #+#    #+#             */
/*   Updated: 2024/06/17 17:30:16 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int env_cmd(t_execcmd *ecmd, t_args *params)
{
	int	i;

	i = 0;
	(void)ecmd;
	while (params->envp[i])
	{
		printf("%d: %s\n", i, params->envp[i]);
		i++;
	}
	return (0);
}