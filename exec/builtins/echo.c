/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabdulki <sabdulki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 17:18:22 by lomakinaval       #+#    #+#             */
/*   Updated: 2024/07/04 00:48:15 by sabdulki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int echo_cmd(t_cmd_info *ecmd)
{
    int n_flag;
    int n_counter;
    int i;

    n_flag = 0;
    i = 1;
    n_counter = 0;
    if (ecmd->argv[1] && ft_strncmp(ecmd->argv[1], "-n", 2) == 0)
    {
        n_flag = 1;
        n_counter++;
        i = 2;
    }
    while (ecmd->argv[i] && ft_strncmp(ecmd->argv[i], "-n", 2) == 0)
    {
        n_counter++;
        i++;
    }
    // if (ecmd->redir_type == APPEND)
    //     return (0);
    while (ecmd->argv[i])
    {
        if ((i > 1 && !n_flag) || (i - n_counter > 1 && n_flag)) // if more than 1 arg and no -n OR if more than 1 arg with -n
        {
            printf(" ");
            // write(1, " ", 1);
        }
        // write(1, ecmd->argv[i], ft_strlen(ecmd->argv[i]));
        printf("%s", ecmd->argv[i]);
        i++;
    }
    if (!n_flag)
    {
        // write(1, "\n", 1);   
        printf("\n");
    }
    return (0);
}