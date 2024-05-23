/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomakinavaleria <lomakinavaleria@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 17:18:22 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/05/23 17:30:52 by lomakinaval      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int echo_cmd(t_execcmd *ecmd)
{
    int n_flag;
    int i;

    n_flag = 0;
    i = 1;
    if (ecmd->argv[1] && ft_strncmp(ecmd->argv[1], "-n", 2) == 0)
    {
        n_flag = 1;
        i = 2;
    }
    while(ecmd->argv[i])
    {
        if ((n_flag && i > 2) || i > 1)
            printf(" ");
        printf("%s", ecmd->argv[i]);
        i++;
    }
    if (!n_flag)
        printf("\n");
    return (0);
}