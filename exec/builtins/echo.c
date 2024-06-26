/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 17:18:22 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/06/24 17:43:50 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int echo_cmd(t_cmd_info *ecmd)
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
    // if (ecmd->redir_type == APPEND)
    //     return (0);
    while (ecmd->argv[i])
    {
        if ((i > 1 && !n_flag) || (i > 2 && n_flag))
            printf(" ");
        printf("%s", ecmd->argv[i]);
        i++;
    }
    if (!n_flag)
        printf("\n");
    return (0);
}